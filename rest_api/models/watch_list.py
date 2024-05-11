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

    class Meta:
        constraints: List[models.BaseConstraint] = [
            models.UniqueConstraint(fields=('user', 'name'), name='unique_user_name'),
        ]


class WatchListDTO(ConvertibleMixin, BaseModel):
    id: Optional[UUID] = Field(alias='uuid', default=None)
    user: Optional[int] = Field(default=None)
    name: str = Field(max_length=80)

    class Config(ConvertibleConfig, BaseConfig):
        ModelType = WatchList

        from_model_rename = {
            'user_id': 'user',
        }
        to_model_rename = {
            'id': 'uuid',
        }

    model_dump = model_dump_wrapper
    model_dump_json = model_dump_json_wrapper
