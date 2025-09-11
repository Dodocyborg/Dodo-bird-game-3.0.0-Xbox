# Cloud Server API Protocol

## Endpoints

### Authentication
- **POST /api/v1/auth/login**  
  Payload: `{ "username": "...", "password": "..." }`  
  Response: `{ "token": "..." }`

### Join Game
- **POST /api/v1/game/join**  
  Payload: `{ "token": "...", "gameId": "..." }`  
  Response: `Joined game!` or error

### Mod Upload
- **POST /api/v1/mods/upload**  
  Payload: `{ "token": "...", "modFile": "..." }`  
  Response: `Success` or error

### Storage
- **POST /api/v1/storage/save**  
  Payload: `{ "token": "...", "data": "..." }`  
  Response: `Success` or error

_All endpoints require TLS encryption and valid authentication tokens._