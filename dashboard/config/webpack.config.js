// Template from https://codechips.me/svelte-and-webpack-5/
const path = require('path')
const { merge } = require('webpack-merge')
const parts = require('./webpack.parts')
const { mode, analyze } = require('webpack-nano/argv')
const CopyPlugin = require("copy-webpack-plugin")
 
const common = (mode) => merge([
  { output: { path: path.resolve(process.cwd(), '../data/www/') } },
  parts.page({ title: 'CANfuck' }),
  //parts.loadImages(),
  parts.loadSvg(),
  parts.svelte(mode),
  parts.extractCSS({ loaders: [parts.postcss()] }),
  parts.cleanDist(),
  parts.useWebpackBar(),
  parts.useDotenv(), 
  {
    resolve: {
      alias: {
        components: path.resolve(process.cwd(), 'src/components/'),
        views: path.resolve(process.cwd(), 'src/views/'),
        assets: path.resolve(process.cwd(), 'public/assets/'),
      },
    },
    plugins: [
      new CopyPlugin({
        patterns: [
          { from: path.resolve(process.cwd(), 'public'), to: path.resolve(process.cwd(), '../data/www') }
        ]
      })
    ]
  }
])

const development = merge([
  { entry: ['./src/main.js', 'webpack-plugin-serve/client'] },
  { target: 'web' },
  parts.generateSourceMaps({ type: 'eval-source-map' }),
  parts.devServer()
])

const production = merge(
  [
    { entry: ['./src/main.js'] },
    parts.optimize(),
    analyze && parts.analyze()
  ].filter(Boolean)
)

const getConfig = mode => {
  switch (mode) {
    case 'production':
      return merge(common(mode), production, { mode })
    case 'development':
      return merge(common(mode), development, { mode })
    default:
      throw new Error(`Unknown mode, ${mode}`)
  }
}

module.exports = getConfig(mode)