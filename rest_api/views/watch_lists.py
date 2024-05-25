import json
import logging
from http import HTTPStatus
from typing import Any, Dict, List, Optional
from uuid import UUID

from django.core.exceptions import ObjectDoesNotExist
from django.core.handlers.wsgi import WSGIRequest
from django.db.models import QuerySet
from django.db.utils import IntegrityError
from django.http import HttpResponse, JsonResponse
from django.views import View
from pydantic import ValidationError

from rest_api.models import WatchList, WatchListDTO
from watch_list_common.datetime_utils import DatetimeUtils, JSONDatetimeEncoder
from watch_list_common.http_utils import JsonErrorResponse
from watch_list_common.http_utils.requests import forbid_resource_id, require_resource_id
from watch_list_common.pydantic_utils import RequestErrorModel


class WatchListsView(View):

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self._logger = logging.getLogger(type(self).__name__)

    @require_resource_id
    def delete(self, request: WSGIRequest, resource_uuid: UUID) -> HttpResponse:
        entity: WatchList
        try:
            entity = WatchList.objects.get(pk=resource_uuid, user=request.user.id)
        except ObjectDoesNotExist:
            return HttpResponse(status=HTTPStatus.NOT_FOUND)

        entity.delete()
        return HttpResponse(status=HTTPStatus.OK)

    def get(self, request: WSGIRequest, resource_uuid: Optional[UUID] = None) -> HttpResponse:
        if resource_uuid is None:
            return self.list(request=request)
        entity: WatchList
        try:
            entity = WatchList.objects.get(pk=resource_uuid, user=request.user.id)
        except ObjectDoesNotExist:
            return HttpResponse(status=HTTPStatus.NOT_FOUND)

        return JsonResponse(status=HTTPStatus.OK, data=WatchListDTO.from_model(entity).model_dump())

    def list(self, request: WSGIRequest) -> HttpResponse:
        data: QuerySet = WatchList.objects.filter(user=request.user.id)
        res_data: List[Dict[str, Any]] = [{
            'uuid': str(entity.uuid),
            **WatchListDTO.from_model(entity).model_dump(exclude={'id', 'user'})
        } for entity in data]

        return HttpResponse(status=HTTPStatus.OK, content_type='application/json', content=json.dumps(res_data, cls=JSONDatetimeEncoder))

    @require_resource_id
    def patch(self, request: WSGIRequest, resource_uuid: UUID) -> HttpResponse:
        return HttpResponse(status=HTTPStatus.NOT_IMPLEMENTED)  # TODO - Implement.

    @forbid_resource_id
    def post(self, request: WSGIRequest) -> HttpResponse:
        create_request: WatchListDTO
        try:
            create_request = WatchListDTO.model_validate_json(request.body)
        except ValidationError as ex:
            self._logger.error(f'Failed to parse request body: [{ex}]')
            return HttpResponse(status=HTTPStatus.BAD_REQUEST)
        if create_request.id is not None:
            self._logger.error('Request data contained resource id')
            return JsonErrorResponse(error_model=RequestErrorModel(detail='request data must not contain resource id'))

        if any(d is not None for d in (create_request.date_created, create_request.date_modified)):
            self._logger.error('Request data contained date_created or date_modified')
            return JsonErrorResponse(error_model=RequestErrorModel(detail='request data must not contain date_created or date_modified'))

        create_request.date_created = DatetimeUtils.now()
        create_request.date_modified = create_request.date_created

        try:
            new_watchlist: WatchList = WatchList.objects.create(user=request.user, **create_request.model_dump())
        except IntegrityError as ex:
            self._logger.error(f'Failed to create watchlist: [{ex}]')
            return JsonErrorResponse(error_model=RequestErrorModel(detail='resource with this name already exists'))

        return JsonResponse(status=HTTPStatus.CREATED, data=WatchListDTO.from_model(new_watchlist).model_dump())
