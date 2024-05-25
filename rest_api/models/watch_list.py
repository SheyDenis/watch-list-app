import datetime
from typing import List, Optional
from uuid import UUID, uuid4

from django.db import models
from pydantic import BaseModel, Field

from rest_api.models import User
from watch_list_common.pydantic_utils import BaseConfig, ConvertibleConfig, ConvertibleMixin, model_dump_json_wrapper, model_dump_wrapper


class WatchList(models.Model):
    uuid = models.UUIDField(primary_key=True, default=uuid4, serialize=True)
    user = models.ForeignKey(User, on_delete=models.CASCADE, editable=False)
    name = models.CharField(max_length=80, editable=True)
    date_created = models.DateTimeField()
    date_modified = models.DateTimeField()

    class Meta:
        constraints: List[models.BaseConstraint] = [
            models.UniqueConstraint(fields=('user', 'name'), name='unique_user_name'),
        ]


class WatchListDTO(ConvertibleMixin, BaseModel):
    id: Optional[UUID] = Field(alias='uuid', default=None)
    user: Optional[int] = Field(default=None)
    name: str = Field(max_length=80)
    date_created: Optional[datetime.datetime] = Field(default=None)
    date_modified: Optional[datetime.datetime] = Field(default=None)

    class Config(BaseConfig, ConvertibleConfig):
        excluded_none_by_default = BaseConfig.excluded_none_by_default | {'date_created', 'date_modified'}
        ModelType = WatchList

        from_model_rename = {
            'user_id': 'user',
        }
        to_model_rename = {
            'id': 'uuid',
        }

    model_dump = model_dump_wrapper
    model_dump_json = model_dump_json_wrapper
