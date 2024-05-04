from django.urls import path

from rest_api.views.users import UsersView

urlpatterns = [
    path('users/', UsersView.as_view()),
]
