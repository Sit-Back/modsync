# Modsync

Modsync helps to synchronise the servers of indecisive owners and their clients.
Having to announce server mod changes and deal with all of the issues that stem from it is in the past.
Modsync aims to fully seamlessly sync mods, mod loaders, and launcher profiles while keeping it as
simple as possible for the average user.

**Modsync needs a sync server to run, one can be found [here](https://github.com/Sit-Back/modsync-server).**

## Build

To build, please set the `SERVER_URL` environment variable to the server address of your sync server. Then create
a new build directory, move inside it and run:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release && cmake --build .
```