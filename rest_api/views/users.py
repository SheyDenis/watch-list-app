import logging
from http import HTTPStatus
from typing import Any, Dict

from django.core.exceptions import ObjectDoesNotExist
from django.core.handlers.wsgi import WSGIRequest
from django.forms.models import model_to_dict
from django.http import HttpResponse, JsonResponse
from django.views import View

from rest_api.models import User


class UsersView(View):

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self._logger = logging.getLogger(type(self).__name__)

    def get(self, request: WSGIRequest):
        user_entity: User
        try:
            user_entity = User.objects.get(pk=request.user.id)
        except ObjectDoesNotExist:
            # This is bad because the user is logged in, so we should be able to fetch it from the DB.
            self._logger.error(f'Failed to fetch user data for user [{request.user.id=}]')
            return HttpResponse(status=HTTPStatus.NOT_FOUND)

        data: Dict[str, Any] = model_to_dict(
            user_entity,
            fields=(
                'email',
                'first_name',
                'last_login',
                'last_name',
                'username',
            ),
        )
        return JsonResponse(data=data, status=HTTPStatus.OK)
