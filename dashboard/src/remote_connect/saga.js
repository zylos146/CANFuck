import { call, cancel, fork, put, select, take, delay } from 'redux-saga/effects';
import dayjs from 'dayjs';

import { EVENT_TYPES } from './constants';
import { subscribeToSSE } from './sse-channel-wrapper';

import * as Actions from './actions'
import {
  REMOTE_HEARTBEAT,
  REMOTE_DATA_POINTS,
  REMOTE_CONNECT,
  REMOTE_DISCONNECT,
  MAX_HEARTBEAT_DIFF_MS
} from './constants'

function* subscribe() {
  console.log('test')
  let esFork, wsFork, heartbeatFork
  yield take(REMOTE_CONNECT)

  while (true) {
    const remoteIp = yield select(state => state.remote.ip);
    console.log(`[remote] Establishing EventSource and WebSocket connection to target ${remoteIp}`)
    esFork = yield fork(subscribeEventSource)
    //wsFork = yield fork(subscribeWebSocket)
    heartbeatFork = yield fork(monitorHeartbeat)

    // Handle connection closing
    const action = yield take([REMOTE_CONNECT, REMOTE_DISCONNECT])

    // Perform the disconnect and shutdown of existing channels
    console.log('[remote] Disconnecting...')
    const { es, ws } = yield select(state => state.channels);
    if (es !== null) { es.close() }
    if (ws !== null) { ws.close() }
    yield cancel(esFork)
    //yield cancel(wsFork)
    yield cancel(heartbeatFork)
    yield put(Actions.disconnected());

    // If we intentionally disconnected, wait for a Connect before we start back up
    switch (action.type) {
      case REMOTE_DISCONNECT:
        console.log('[remote] Disconnected')
        yield take(REMOTE_CONNECT)
        break
      case REMOTE_CONNECT:
        console.log('[remote] Attempting to re-establish connection...')
        break
    }

    yield delay(100);
  }
}

function* subscribeEventSource() {
  try {
    const remoteIp = yield select(state => state.remote.ip);
    const es = new EventSource(`http://${remoteIp}/es`);
    const esChannel = yield call(subscribeToSSE, es);
    yield put(Actions.setEventSource(es));

    while (true) {
      const event = yield take(esChannel);
      switch (event.type) {
        case EVENT_TYPES.HEARTBEAT:
          const data = JSON.parse(String(event.data));
          yield put(Actions.receivedHeartbeat(dayjs()));
          break
      }
    }
  } catch (error) {
    console.error(error);
  }
}

function* subscribeWebSocket() {
  try {
    const remoteIp = yield select(state => state.remote.ip);
    const ws = new WebSocket(`ws://${remoteIp}`);
    const wsChannel = yield call(subscribeToSSE, ws);
    yield put(Actions.setWebSocket(ws));

    while (true) {
      const event = yield take(wsChannel);
      if (event.type === EVENT_TYPES.HEARTBEAT) {
        const data = JSON.parse(String(event.data));
        yield put(Actions.receivedHeartbeat(dayjs()));
      }
    }
  } catch (error) {
    console.error(error);
  }
}

function* monitorHeartbeat() {
  try {
    while (true) {
      yield delay(100);
      
      const esChannel = yield select(state => state.channels.es);
      const lastHeartbeat = yield select(state => state.lastHeartbeat);
      if (esChannel !== null && lastHeartbeat !== null) {
        const diffMs = dayjs().diff(lastHeartbeat)
        if (diffMs > MAX_HEARTBEAT_DIFF_MS) {
          console.info(`[remote.heartbeat] Have not received a heartbeat from remote device in over ${diffMs}ms. Attempting reconnection...`)
          yield put(Actions.setHealth(false));
          yield put(Actions.startConnection());
          break
        } else {
          yield put(Actions.setHealth(true));
        }
      }
    }
  } catch (error) {
    console.error(error);
  }
}

// eslint-disable-next-line
export default [subscribe];