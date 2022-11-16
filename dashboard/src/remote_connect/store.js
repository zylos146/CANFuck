import {
  REMOTE_SET_IP,
  REMOTE_DISCONNECTED
} from "./constants"

const initialState = {
  channels: {
    ws: null,
    es: null
  },
  lastHeartbeat: null,
  maxHeartbeatDiff: 500,
  ip: localStorage.getItem('remote.ip'),
  isHealthy: false
}

export default function counter(state = initialState, action) {
  switch (action.type) {
    case REMOTE_SET_IP:
      localStorage.setItem('remote.ip', action.ip)
      return {
        ...state,
        ip: action.ip
      }
    case REMOTE_DISCONNECTED:
      return {
        ...state,
        lastHeartbeat: null,
        channels: {
          ws: null,
          es: null
        }
      }
    case 'INCREMENT':
      return state + 1
    case 'DECREMENT':
      return state - 1
    default:
      return state
  }
}