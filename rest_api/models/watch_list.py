from typing import List, Optional
from uuid import UUID, uuid4

from django.db import models
from pydantic import BaseModel, Field

from rest_api.models import User


class WatchList(models.Model):
    uuid = models.UUIDField(primary_key=True, default=uuid4)
    user = models.ForeignKey(User, on_delete=models.CASCADE, editable=False)
    name = models.CharField(max_length=80, editable=True)

    class Meta:
        constraints: List[models.BaseConstraint] = [
            models.UniqueConstraint(fields=('user', 'name'), name='unique_user_name'),
        ]


class WatchListDTO(BaseModel):
    id: Optional[UUID] = Field(alias='uuid', default=None)
    user: Optional[int] = Field(default=None)
    name: str = Field(max_length=80)
