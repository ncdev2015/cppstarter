## Generate static library (By default)

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## Generate dynamic library (By default)

```bash
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON ..
```