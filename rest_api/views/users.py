import logging
from typing import List

from django.core.handlers.wsgi import WSGIRequest
from django.db.models import QuerySet
from django.forms.models import model_to_dict
from django.http import JsonResponse
from django.views import View

from rest_api.models import User


class UsersView(View):

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self._logger = logging.getLogger(type(self).__name__)

    def get(self, request: WSGIRequest):
        # FIXME - Return info about the currently authenticated user. <WLA-22>
        data: QuerySet = User.objects.all()
        data_list: List[User] = [model_to_dict(m) for m in data]
        return JsonResponse(data=data_list, status=200, safe=False)
