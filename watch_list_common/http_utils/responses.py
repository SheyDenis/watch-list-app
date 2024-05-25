from http import HTTPStatus

from django.http import JsonResponse

from watch_list_common.pydantic_utils.models import RequestErrorModel


class JsonErrorResponse(JsonResponse):
    """Generic JSON error response."""

    def __init__(self, *args, error_model: RequestErrorModel, status: HTTPStatus = HTTPStatus.BAD_REQUEST, **kwargs):
        super().__init__(data=error_model.model_dump(), status=status, *args, **kwargs)
