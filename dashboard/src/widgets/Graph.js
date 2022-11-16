import React from 'react'
import PropTypes from 'prop-types'
import ResizeObserver  from 'react-resize-observer'

import Plotly from 'plotly.js-dist'

export default class Graph extends React.Component {
  constructor(props) {
    super(props)

		this.graphRef = React.createRef()
    this.cnt = 0;
    this.sTime = 0;
  }

  render() {
		return <>
      <ResizeObserver onResize={() => {}} />
      <div className={this.props.className} ref={this.graphRef}/>
    </>
	}

	componentDidMount() {
		if (this.graphRef.current) {
      let DATA_INTERVAL
      let PERIOD_SECONDS = 2;
      let POINTS_CAPTURED = PERIOD_SECONDS
  
      let xData = [];
      let yData = [];
      for (let i = 0; i < 500; i++) {
        xData.push((i - 500) * 16 / 1000);
        yData.push(0);
      }
  
      Plotly.newPlot(this.graphRef.current, [{
        x: xData,
        y: yData,
        name: 'Actual Position',
        yaxis: 'y'
      }, {
        x: xData,
        y: yData,
        name: 'Actual Velocity',
        yaxis: 'y2'
      }, {
        x: xData,
        y: yData,
        name: 'Demand Position',
        yaxis: 'y3'
      }], {
        margin: { t: 0 },
        xaxis: { range: [-PERIOD_SECONDS, 0], showticklabels: false }
      });

      setInterval(function() {
        Plotly.relayout(this.graphRef.current,{
          xaxis: {
            range: [this.sTime - 1, this.sTime],
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
      
        this.cnt++;
        this.sTime += 3 / 1000;
      }, 3);
		}
	}
};