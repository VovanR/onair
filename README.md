# onair

> Webcamera and Microphone onair status

## Start server

Install dependencies
```sh
npm install
```

Start the server
```sh
npm start
```

## Work with API

### Get status

```sh
curl http://localhost:3000/api/status
```

### Set status

```sh
curl -d '{"camera":true,"microphone":true}' -X POST http://localhost:3000/api/status -H "Content-Type: application/json"
```


## PC

### Shell script

```shell
mkdir -p ~/.local/bin/
cp bin/com.vovanr.onair.sh ~/.local/bin/
```


### Systemd

```shell
mkdir -p ~/.config/systemd/user/
cp systemd/* ~/.config/systemd/user/
systemctl --user enable com.vovanr.onair.timer
systemctl --user start com.vovanr.onair.timer
```
