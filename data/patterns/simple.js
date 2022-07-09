addPattern("Simple Stroke", {
  isOutStroke: false
}, function (timestamp, isAtTarget) {
  var position = 0

  if (isAtTarget) {
    this.isOutStroke = !this.isOutStroke

    if (this.isOutStroke) {
      position = this.stroke
    } else {
      position = 0
    }
  }

  return {
    position,
    speed: this.speed,
    acceleration: this.acceleration
  }
})