import uuid
from typing import List

from django.db import models

from rest_api.models import User


class WatchList(models.Model):
    uuid = models.UUIDField(primary_key=True, default=uuid.uuid4)
    user = models.ForeignKey(User, on_delete=models.CASCADE, editable=False)
    name = models.CharField(max_length=80, editable=True)

    class Meta:
        constraints: List[models.BaseConstraint] = [
            models.UniqueConstraint(fields=('user', 'name'), name='unique_user_name'),
        ]
