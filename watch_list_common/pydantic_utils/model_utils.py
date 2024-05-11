from typing import Any, Dict, Optional, Set, cast

from pydantic import BaseModel


class BaseConfig:
    """Base config class for pydantic models."""
    validate_assignment = True
    extra = 'forbid'

    # Add 'by_alias' as default when dumping model.
    by_alias_by_default: bool = True

    # Add exclude properties by default if they are None.
    excluded_none_by_default: Set[str] = ('id', 'user')

    # Properties to always be excluded when dumping models.
    always_exclude: Set[str] = set()


def model_dump_wrapper(obj: BaseModel, by_alias: bool = True, **kwargs) -> Dict[str, Any]:
    if not issubclass(obj.Config, BaseConfig):
        raise ValueError(f'Object [{type(obj).__name__}] must have a config of type [{BaseConfig.__name__}] has [{obj.Config.__name__}]')
    if obj.Config.by_alias_by_default and 'by_alias' not in kwargs:
        kwargs['by_alias'] = by_alias

    excludes = __generate_excludes(obj, kwargs.get('exclude', None), kwargs.get('include', None))
    if len(excludes) > 0:
        kwargs['exclude'] = excludes
    return super(type(obj), obj).model_dump(**kwargs)


def model_dump_json_wrapper(obj: BaseModel, by_alias: bool = True, **kwargs) -> str:
    if not issubclass(obj.Config, BaseConfig):
        raise ValueError(f'Object [{type(obj).__name__}] must have a config of type [{BaseConfig.__name__}] has [{obj.Config.__name__}]')

    if obj.Config.by_alias_by_default and 'by_alias' not in kwargs:
        kwargs['by_alias'] = by_alias

    excludes = __generate_excludes(obj, kwargs.get('exclude', None), kwargs.get('include', None))
    if len(excludes) > 0:
        kwargs['exclude'] = excludes

    return super(type(obj), obj).model_dump_json(**kwargs)


def __generate_excludes(obj: BaseModel, exclude: Optional[Set[str]], include: Optional[Set[str]]) -> Set[str]:
    excludes: Set[str] = set()
    if exclude:
        excludes = exclude
    if include is None:
        include = set()

    config = cast(BaseConfig, obj.Config)
    if exclude is None:
        for prop in config.excluded_none_by_default:
            if prop in include:
                continue
            if hasattr(obj, prop) and getattr(obj, prop) is None:
                excludes.add(prop)
    for prop in config.always_exclude:
        if prop in include:
            raise RuntimeError(f'Property [{prop}] was included but marked as always excluded')
        if hasattr(obj, prop) and getattr(obj, prop) is None:
            excludes.add(prop)
    return excludes
