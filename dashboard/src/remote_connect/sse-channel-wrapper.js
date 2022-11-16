import { END, eventChannel } from 'redux-saga'
import { EVENT_TYPES } from './constants'

// From https://mohit5.medium.com/server-sent-events-with-typescript-and-redux-sagas-b90443128e13
export function subscribeToSSE(eventSrc) {
  return eventChannel((emitter) => {
    eventSrc.onopen = (ev) => {
      console.info('[es] connection is established');
      emitter(ev);
    };

    eventSrc.onerror = (err) => {
      console.error('[es] connection error', err);
    };

    eventSrc.addEventListener(EVENT_TYPES.POINTS, (ev) => {
      console.info('[es] data event recieved...', ev);
      emitter(ev);
    });

    eventSrc.addEventListener(EVENT_TYPES.HEARTBEAT, (ev) => {
      console.info('[es] data event recieved...', ev);
      emitter(ev);
    });

    return () => {
      console.info('[es]closing connection...');
      eventSrc.close();
      emitter(END);
    };
  });
}