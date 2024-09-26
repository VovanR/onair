# onair

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
curl -d '{"status":true}' -X POST http://localhost:3000/api/status -H "Content-Type: application/json"
```
