from django.contrib import admin
from django.contrib.auth.admin import UserAdmin

from rest_api.models import User


class CustomUserAdmin(UserAdmin):
    pass


admin.site.register(User, CustomUserAdmin)
