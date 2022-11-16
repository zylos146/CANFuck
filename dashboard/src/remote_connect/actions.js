import {
  REMOTE_HEARTBEAT,
  REMOTE_DATA_POINTS,
  REMOTE_CONNECT,
  REMOTE_DISCONNECT,
  REMOTE_DISCONNECTED,
  REMOTE_SET_HEALTH,
  REMOTE_SET_IP,
  REMOTE_ES_SET,
  REMOTE_WS_SET
} from './constants'

export function receivedHeartbeat(lastHeartbeat) {
   return {
      type: REMOTE_HEARTBEAT,
      lastHeartbeat
   }
}


export function receivedDataPoints(points) {
   return {
      type: REMOTE_DATA_POINTS,
      points
   }
}

export const startConnection = () => ({ type: REMOTE_CONNECT })
export const disconnect = () => ({ type: REMOTE_DISCONNECT })
export const disconnected = () => ({ type: REMOTE_DISCONNECTED })

export const setHealth = (isHealthy) => ({ type: REMOTE_SET_HEALTH, isHealthy })
export const setRemoteIp = (ip) => ({ type: REMOTE_SET_IP, ip })
export const setEventSource = (channel) => ({ type: REMOTE_ES_SET, channel })
export const setWebSocket = (channel) => ({ type: REMOTE_WS_SET, channel })
