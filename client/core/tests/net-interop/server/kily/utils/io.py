import io
import os


def readf(filepath: str) -> bytes:
    with io.FileIO(filepath, 'rb') as f:
        return f.readall()


def ensure_path(path: str):
    if not os.path.exists(path):
        os.makedirs(path)


def writef(filepath: str, contents: bytes):
    with io.FileIO(filepath, 'wb') as f:
        f.write(contents)