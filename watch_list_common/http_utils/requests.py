from functools import wraps
from http import HTTPStatus

from watch_list_common.http_utils.responses import JsonErrorResponse
from watch_list_common.pydantic_utils import RequestErrorModel


def forbid_resource_id(view_func):
    """Decorator to forbid resource id in the request URL.

    Useful when a django.views.View has a path with AND without resource id in them.
    Example:
        watchlists/        -> Used to create a new WatchList (POST).
        watchlists/<uuid>  -> Used to delete a WatchLists (DELETE).

    In this case, Django will forward requests with and without <uuid> to the 'create' handler event though the handler
    doesn't accept/support it.
    """

    @wraps(view_func)
    def _wrapped_view(self, request, resource_uuid=None, *args, **kwargs):
        if resource_uuid is not None:
            return JsonErrorResponse(status=HTTPStatus.BAD_REQUEST,
                                     error_model=RequestErrorModel(detail='method not supported with resource id'))
        return view_func(self, request, *args, **kwargs)

    return _wrapped_view


def require_resource_id(view_func):
    """Decorator to require resource id in the request URL.

    Useful when a django.views.View has a path with AND without resource id in them.
    Example:
        watchlists/        -> Used to create a new WatchList (POST).
        watchlists/<uuid>  -> Used to delete a WatchLists (DELETE).

    In this case, Django will forward requests with and without <uuid> to the 'delete' handler event though the handler requires it.
    """

    @wraps(view_func)
    def _wrapped_view(self, request, resource_uuid=None, *args, **kwargs):
        if resource_uuid is None:
            return JsonErrorResponse(status=HTTPStatus.BAD_REQUEST, error_model=RequestErrorModel(detail='method requires resource id'))
        return view_func(self, request, resource_uuid=resource_uuid, *args, **kwargs)

    return _wrapped_view
