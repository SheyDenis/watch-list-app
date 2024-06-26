def str2bool(s: str) -> bool:
    """Convert a string to a boolean."""
    if isinstance(s, bool):
        return s
    if not isinstance(s, str):
        raise TypeError(f'Argument must be a string, got [{type(s).__name__}]')
    s_lower = s.lower()
    return s_lower in ('y', 'yes', 'true', '1')
