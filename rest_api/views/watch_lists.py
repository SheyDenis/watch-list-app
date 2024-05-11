import json
import logging
from http import HTTPStatus
from typing import Any, Dict, List, Optional
from uuid import UUID

from django.core.exceptions import ObjectDoesNotExist
from django.core.handlers.wsgi import WSGIRequest
from django.db.models import QuerySet
from django.forms.models import model_to_dict
from django.http import HttpResponse, JsonResponse
from django.views import View

from rest_api.models import WatchList


class WatchListsView(View):

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self._logger = logging.getLogger(type(self).__name__)

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

        return JsonResponse(data=model_to_dict(entity), status=HTTPStatus.OK)

    def list(self, request: WSGIRequest) -> HttpResponse:
        data: QuerySet = WatchList.objects.filter(user=request.user.id)
        res_data: List[Dict[str, Any]] = [{
            'uuid': str(entity.uuid),
            **model_to_dict(
                entity,
                exclude=('uuid',),
            ),
        } for entity in data]
        return HttpResponse(status=HTTPStatus.OK, content_type='application/json', content=json.dumps(res_data))

    def patch(self, request: WSGIRequest, resource_uuid: UUID) -> HttpResponse:
        return HttpResponse(HTTPStatus.NOT_IMPLEMENTED)  # TODO - Implement.

    def post(self, request: WSGIRequest) -> HttpResponse:
        return HttpResponse(HTTPStatus.NOT_IMPLEMENTED)  # TODO - Implement.

    def put(self, request: WSGIRequest, resource_uuid: UUID) -> HttpResponse:
        return HttpResponse(HTTPStatus.NOT_IMPLEMENTED)  # TODO - Implement.
