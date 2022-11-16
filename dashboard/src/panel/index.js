import React from 'react'
import { Responsive, WidthProvider } from "react-grid-layout"
import ResizeObserver  from 'react-resize-observer'

import './panel.css';
import Graph from '../widgets/Graph'
import XTerm from '../widgets/XTerm'
import RemoteConnect from '../remote_connect'
const ResponsiveReactGridLayout = WidthProvider(Responsive);

export default class App extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      currentBreakpoint: "lg",
      mounted: false,
      layouts: { lg: [
        { i: "a", x: 0, y: 0, w: 12, h: 2, static: true },
        { i: "b", x: 1, y: 0, w: 12, h: 12 },
        { i: "c", x: 4, y: 0, w: 12, h: 12 }
      ]}
    };

    this.onBreakpointChange = this.onBreakpointChange.bind(this);
    this.onLayoutChange = this.onLayoutChange.bind(this);
  }

  render() {
    // <Graph className='graph'/>
    return <>
      <ResizeObserver onResize={() => {}} />
      <ResponsiveReactGridLayout
        {...this.props}
        layouts={this.state.layouts}

        onBreakpointChange={this.onBreakpointChange}
        onLayoutChange={this.onLayoutChange}
        // WidthProvider option
        measureBeforeMount={false}
        // I like to have it animate on mount. If you don't, delete `useCSSTransforms` (it's default `true`)
        // and set `measureBeforeMount={true}`.
        useCSSTransforms={this.state.mounted}
        compactType={this.state.compactType}
        preventCollision={!this.state.compactType}
      >
        <div key="a">
          <div className="status-bar">
            Displayed as <code>[x, y, w, h]</code>: 
            <RemoteConnect/>
            <div className="columns"></div>
          </div>
        </div>
        <div key="b"></div>
        <div key="c"><XTerm className="xterm"/></div>
      </ResponsiveReactGridLayout>
    </>;
  }

  onLayoutChange(layout, layouts) {}
  onBreakpointChange(breakpoint) {
    this.setState({
      currentBreakpoint: breakpoint
    });
  }
}

App.defaultProps = {
  className: "layout",
  rowHeight: 30,
  onLayoutChange: function() {},
  cols: { lg: 12, md: 10, sm: 6, xs: 4, xxs: 2 }
};