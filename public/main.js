let deviceIp = '192.168.137.252';
let lastHeartbeat = dayjs();
let deviceES;
let deviceWS;
let term;

let cnt = 0;
let sTime = 0;
function documentLoaded() {
  TESTER = document.getElementById('tester');

  let DATA_INTERVAL
  let PERIOD_SECONDS = 2;
  let POINTS_CAPTURED = PERIOD_SECONDS

  let xData = [];
  let yData = [];
  for (let i = 0; i < 500; i++) {
    xData.push((i - 500) * 16 / 1000);
    yData.push(0);
  }

  Plotly.newPlot('tester', [{
    x: xData,
    y: yData,
    name: 'Actual Position',
    yaxis: 'y'
  }, {
    x: xData,
    y: yData,
    name: 'Demand Position',
    yaxis: 'y2'
  }, {
    x: xData,
    y: yData,
    name: 'Actual Velocity',
    yaxis: 'y3'
  }], {
    margin: { t: 0 },
    xaxis: { range: [-PERIOD_SECONDS, 0], showticklabels: false }
  });

  setInterval(function(){
    Plotly.relayout('tester',{
      xaxis: {
        range: [sTime - 1, sTime],
        showticklabels: false,
        domain: [0.0, 0.9]
      },
      yaxis: {
        title: 'Position',
        range: [0, 100]
      },
      yaxis2: {
        title: 'Velocity',
        range: [0, 10],
        anchor: 'x',
        overlaying: 'y',
        side: 'right'
      },
      yaxis3: {
        title: 'Force',
        range: [0, 10],
        overlaying: 'y',
        anchor: 'free',
        side: 'right',
        position: 0.98
      }
    });
  
    cnt++;
    sTime += 3 / 1000;
  }, 3);

  term = new Terminal();
  term.open(document.getElementById('terminal'));
  term.resize(160, 40)
  term.write('Hello from \x1B[1;3;31mxterm.js\x1B[0m')

  setInterval(() => {
    if (dayjs().diff(lastHeartbeat, 'ms') > 500) {
      console.log('No heartbeat within 500ms. Attempting to re-connect to device!')
      connectDevice()
    }
  }, 500);
}

let traceKeys = [
  "apos",
  "avel",
  "aforce",

  "dpos",
  "dvel",

  "vsp",
  "wloss",
  "mtemp",
  "rtemp"
]

let lastTimeSync = dayjs()
let connectDevice = () => {
  if (deviceWS) {
    deviceWS.close();
  }
  if (deviceES) {
    deviceES.close();
  }

  deviceWS = new WebSocket(`ws://${deviceIp}`);
  deviceES = new EventSource(`http://${deviceIp}/es`);

  deviceWS.onmessage = function(event) {
    const msg = JSON.parse(event.data);
    console.log(`[ws] ${event.data}`);

    term.write(`${dayjs().format('HH:mm:ss.SSS')} ${msg.msg}`)
  }

  deviceES.addEventListener('open', function(e) {
    console.log("[es] Connected")
  }, false);

  deviceES.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("[es] Disconnected", e)
    }
  }, false);

  deviceES.addEventListener('points', function(e) {
    console.log("[es] points", e.data)

    const points = JSON.parse(e.data)
    for (const key in points) {
      let tId = traceKeys.findIndex((value) => value == key);
      let xData = []
      let yData = []

      for (const point of points[key]) {
        xData.push([sTime])
        yData.push([point[1]])
      }

      if (tId == -1) {
        continue;
      }

      Plotly.extendTraces('tester',{ 
        x: xData,
        y: yData
      }, [tId], 500);
    }
  }, false);

  deviceES.addEventListener('heartbeat', function(e) {
    lastHeartbeat = dayjs()
  }, false);
}