from setuptools import Extension, setup

module = Extension("symnmfAPI", sources=["symnmf.c", "symnmfmodule.c"])
setup(
    name="symnmfAPI",
    version="1.0",
    description="Python wrapper for symnmf C extension.",
    ext_modules=[module]
)
