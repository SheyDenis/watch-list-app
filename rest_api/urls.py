from django.urls import path

from rest_api.views import UsersView, WatchListsView

urlpatterns = [
    path('users/', UsersView.as_view()),
    path('watch_lists/', WatchListsView.as_view()),
    path('watch_lists/<uuid:resource_uuid>/', WatchListsView.as_view()),
]
