from http import HTTPStatus

from django.contrib.auth import authenticate, login, logout
from django.core.handlers.wsgi import WSGIRequest
from django.http import HttpResponse
from django.views import View


class SessionsView(View):

    def post(self, request: WSGIRequest):
        if not all(k in request.POST for k in ('username', 'password')):
            return HttpResponse(status=HTTPStatus.BAD_REQUEST)

        username = request.POST['username']
        password = request.POST['password']
        user = authenticate(request, username=username, password=password)
        if user is not None:
            login(request, user)
            return HttpResponse(status=HTTPStatus.OK)
        return HttpResponse(status=HTTPStatus.UNAUTHORIZED)

    def delete(self, request: WSGIRequest):
        logout(request)
        return HttpResponse(status=HTTPStatus.OK)
