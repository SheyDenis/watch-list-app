import re
from http import HTTPStatus

from django.conf import settings
from django.core.handlers.wsgi import WSGIRequest
from django.http import HttpResponse


class LoginRequiredMiddleware:
    """Middleware requiring that the user would be logged in when making API calls."""

    def __init__(self, get_response):
        self.__get_response = get_response
        self.__required_paths = tuple(re.compile(url) for url in settings.LOGIN_REQUIRED_URLS)

    def __call__(self, request: WSGIRequest):
        if not request.user.is_authenticated:
            if any(ptrn.match(request.path) for ptrn in self.__required_paths):
                return HttpResponse(status=HTTPStatus.UNAUTHORIZED)

        return self.__get_response(request)

    def process_view(self, request, view_func, view_args, view_kwargs):
        return None
