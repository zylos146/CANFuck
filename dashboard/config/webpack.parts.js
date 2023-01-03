const path = require('path')
const { MiniHtmlWebpackPlugin } = require('mini-html-webpack-plugin')
const MiniCssExtractPlugin = require('mini-css-extract-plugin')
const DotenvPlugin = require('dotenv-webpack')
const { WebpackPluginServe } = require('webpack-plugin-serve')
const { CleanWebpackPlugin } = require('clean-webpack-plugin')
const WebpackBar = require('webpackbar')
const { BundleAnalyzerPlugin } = require('webpack-bundle-analyzer')
const CssMinimizerPlugin = require('css-minimizer-webpack-plugin')
const preprocess = require('svelte-preprocess')

exports.devServer = () => ({
  watch: true,
  plugins: [
    new WebpackPluginServe({
      port: 3000,
      static: path.resolve(process.cwd(), '../data/www'),
      historyFallback: {
        htmlAcceptHeaders: ["text/html", "*/*"],
        rewrites: []
     },
     host: 'localhost'
    })
  ]
})

exports.page = ({ title }) => ({
  plugins: [new MiniHtmlWebpackPlugin({ publicPath: '/', context: { title } })]
})

exports.generateSourceMaps = ({ type }) => ({ devtool: type })

exports.svelte = mode => {
  const prod = mode === 'production'

  return {
    resolve: {
      alias: {
        svelte: path.dirname(require.resolve('svelte/package.json'))
      },
      extensions: ['.mjs', '.js', '.svelte', '.ts'],
      mainFields: ['svelte', 'browser', 'module', 'main']
    },
    module: {
      rules: [
        {
          test: /\.svelte$/,
          use: {
            loader: 'svelte-loader',
            options: {
              compilerOptions: {
                dev: !prod
              },
              emitCss: prod,
              hotReload: !prod,
              preprocess: preprocess({
                postcss: true
              })
            }
          }
        },
        {
          test: /node_modules\/svelte\/.*\.mjs$/,
          resolve: {
            fullySpecified: false
          }
        },
      ]
    }
  }
}

exports.postcss = () => ({
  loader: 'postcss-loader'
})

exports.extractCSS = ({ options = {}, loaders = [] } = {}) => {
  return {
    module: {
      rules: [
        {
          test: /\.(p?css)$/,
          use: [{ loader: MiniCssExtractPlugin.loader, options }, 'css-loader'].concat(
            loaders
          ),
          sideEffects: true
        }
      ]
    },
    plugins: [
      new MiniCssExtractPlugin({
        filename: '[name].css'
      })
    ]
  }
}

exports.loadImages = ({ limit } = {}) => ({
  module: {
    rules: [
      {
        test: /\.(png|jpg|gif|webp)$/,
        type: 'asset',
        parser: { dataUrlCondition: { maxSize: limit } }
      }
    ]
  }
})

exports.loadSvg = () => ({
  module: { rules: [{ test: /\.svg$/, type: 'asset' }] }
})

exports.useDotenv = () => ({
  plugins: [new DotenvPlugin()]
})

// clean dist directory on build
exports.cleanDist = () => ({
  plugins: [new CleanWebpackPlugin()]
})

// show a nice progress bar in the terminal
exports.useWebpackBar = () => ({
  plugins: [new WebpackBar()]
})

exports.optimize = () => ({
  optimization: {
    minimize: true,
    splitChunks: { chunks: 'all' },
    runtimeChunk: { name: 'runtime' },
    minimizer: [`...`, new CssMinimizerPlugin()]
  }
})

exports.analyze = () => ({
  plugins: [
    new BundleAnalyzerPlugin({
      generateStatsFile: true
    })
  ]
})