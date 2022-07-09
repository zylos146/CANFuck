package obj
{
   import flash.geom.Point;
   import flash.utils.Dictionary;
   
   public class AutomaticControl
   {
      
      public static var autoModeNameList:Array = ["Normal","Soft","Hard","Self"];
       
      
      private var t:uint = 0;
      
      private var nextModeTimer:int = 0;
      
      private var randomMode:Function;
      
      private var transitioning:Boolean = false;
      
      private var transitionSpeed:Number = 0.01;
      
      private var modeRatio:Number;
      
      private var currentPos:Point;
      
      private var lastPos:Point;
      
      private var currentPenisTipPos:Number = 0;
      
      private var fullLength:Number = 1.0;
      
      private var lastMode:Function;
      
      private var currentMode:Function;
      
      private var singleAction:Function;
      
      private var waitingForSingleAction:Boolean = false;
      
      private var singleActionTimer:uint = 0;
      
      private var singleActionTime:uint = 0;
      
      private var standardModes:Array;
      
      private var softModes:Array;
      
      private var hardModes:Array;
      
      private var selfModes:Array;
      
      private var standardHJModes:Array;
      
      private var softHJModes:Array;
      
      private var hardHJModes:Array;
      
      private var selfHJModes:Array;
      
      private var currentSelfModes:Array;
      
      private var singleActions:Array;
      
      private var happySingleActions:Array;
      
      private var singleActionModes:Dictionary;
      
      private var lastModeNo:uint;
      
      private var offTilt:Number = 0;
      
      private var offTiltTarget:Number = 0;
      
      private var offTiltChangeTimer:uint = 0;
      
      private var ejaculatingModes:Array;
      
      private var normalEjacModes:Array;
      
      private var hardEjacModes:Array;
      
      private var softEjacModes:Array;
      
      private var selfEjacModes:Array;
      
      private var lastEjacMode:Function;
      
      private var deepthroatTarget:Number = 0;
      
      private var deepthroatProgress:Number = 0;
      
      private var usingDeepSlow:Boolean = false;
      
      private var usingFullSlow:Boolean = false;
      
      public function AutomaticControl()
      {
         this.currentPos = new Point(0,0);
         this.lastPos = new Point(0,0);
         super();
         this.standardModes = new Array();
         this.standardModes[0] = this.holdDown;
         this.standardModes[1] = this.activeHoldDown;
         this.standardModes[2] = this.offHold;
         this.standardModes[3] = this.fullSlow;
         this.standardModes[4] = this.fullFast;
         this.standardModes[5] = this.deepFast;
         this.standardModes[6] = this.shallowSlow;
         this.standardModes[7] = this.shallowMedium;
         this.standardModes[8] = this.shallowFast;
         this.standardModes[9] = this.mediumSlow;
         this.standardModes[10] = this.tipToDeepMedium;
         this.standardModes[11] = this.mediumFast;
         this.softModes = new Array();
         this.softModes[0] = this.offHold;
         this.softModes[1] = this.shallowSlow;
         this.softModes[2] = this.shallowMedium;
         this.softModes[3] = this.mediumSlow;
         this.softModes[4] = this.tipToDeepMedium;
         this.softModes[5] = this.tipToDeepSlow;
         this.hardModes = new Array();
         this.hardModes[0] = this.holdDown;
         this.hardModes[1] = this.activeHoldDown;
         this.hardModes[2] = this.offHold;
         this.hardModes[3] = this.fullFast;
         this.hardModes[4] = this.deepFast;
         this.hardModes[5] = this.shallowFast;
         this.hardModes[6] = this.mediumFast;
         this.hardModes[7] = this.tipToDeepFast;
         this.hardModes[8] = this.fullSlam;
         this.hardModes[9] = this.hiltMove;
         this.selfModes = new Array();
         this.selfModes[0] = this.offHold;
         this.selfModes[1] = this.shallowSlow;
         this.selfModes[2] = this.shallowMedium;
         this.selfModes[3] = this.shallowMedium;
         this.selfModes[4] = this.mediumSlow;
         this.selfModes[5] = this.tipToDeepSlow;
         this.selfModes[6] = this.tipToDeepMedium;
         this.selfModes[7] = this.tipToDeepMedium;
         this.selfModes[8] = this.tipHold;
         this.selfModes[9] = this.outsideLickBalls;
         this.standardHJModes = new Array();
         this.standardHJModes[0] = this.fullSlow;
         this.standardHJModes[1] = this.fullFast;
         this.standardHJModes[2] = this.deepFast;
         this.standardHJModes[3] = this.shallowSlow;
         this.standardHJModes[4] = this.shallowMedium;
         this.standardHJModes[5] = this.shallowFast;
         this.standardHJModes[6] = this.mediumSlow;
         this.standardHJModes[7] = this.tipToDeepMedium;
         this.standardHJModes[8] = this.mediumFast;
         this.softHJModes = new Array();
         this.softHJModes[0] = this.shallowSlow;
         this.softHJModes[1] = this.shallowMedium;
         this.softHJModes[2] = this.mediumSlow;
         this.softHJModes[3] = this.tipToDeepMedium;
         this.softHJModes[4] = this.tipToDeepSlow;
         this.hardHJModes = new Array();
         this.hardHJModes[0] = this.fullFast;
         this.hardHJModes[1] = this.deepFast;
         this.hardHJModes[2] = this.shallowFast;
         this.hardHJModes[3] = this.mediumFast;
         this.hardHJModes[4] = this.tipToDeepFast;
         this.hardHJModes[5] = this.fullSlam;
         this.selfHJModes = new Array();
         this.selfHJModes[0] = this.shallowSlow;
         this.selfHJModes[1] = this.shallowMedium;
         this.selfHJModes[2] = this.shallowMedium;
         this.selfHJModes[3] = this.mediumSlow;
         this.selfHJModes[4] = this.tipToDeepSlow;
         this.selfHJModes[5] = this.tipToDeepMedium;
         this.selfHJModes[6] = this.tipToDeepMedium;
         this.singleActions = new Array();
         this.singleActions[0] = this.oneDeep;
         this.happySingleActions = new Array();
         this.happySingleActions[0] = this.waggleEyebrows;
         this.happySingleActions[1] = this.waggleEyebrowsWithTongue;
         this.happySingleActions[2] = this.oneDeep;
         this.singleActionModes = new Dictionary();
         this.singleActionModes[this.waggleEyebrows] = this.offHold;
         this.singleActionModes[this.waggleEyebrowsWithTongue] = this.offHold;
         this.singleActionModes[this.oneDeep] = this.offHold;
         this.normalEjacModes = new Array();
         this.normalEjacModes[0] = this.activeHoldDown;
         this.normalEjacModes[1] = this.shallowHold;
         this.normalEjacModes[2] = this.offHold;
         this.normalEjacModes[3] = this.offHold;
         this.softEjacModes = new Array();
         this.softEjacModes[0] = this.shallowHold;
         this.softEjacModes[1] = this.offHold;
         this.softEjacModes[2] = this.offHold;
         this.hardEjacModes = new Array();
         this.hardEjacModes[0] = this.activeHoldDown;
         this.hardEjacModes[1] = this.shallowHold;
         this.hardEjacModes[2] = this.offHold;
         this.hardEjacModes[3] = this.offHold;
         this.hardEjacModes[4] = this.hiltMove;
         this.selfEjacModes = new Array();
         this.selfEjacModes[0] = this.tipHold;
         this.selfEjacModes[1] = this.shallowHold;
         this.selfEjacModes[2] = this.offHold;
         this.selfEjacModes[3] = this.offHold;
         this.currentMode = this.noMovement;
         this.ejaculatingModes = this.normalEjacModes;
         this.randomMode = this.standardAuto;
         EventBus.addListener("penisTipPosChanged",this.updatePenisTipPos);
      }
      
      private function updatePenisTipPos() : void
      {
         this.currentPenisTipPos = g.animationControl.estimateTipPos();
         this.fullLength = 1 - this.currentPenisTipPos;
      }
      
      public function findAutoMode(param1:String) : void
      {
         var _loc2_:uint = autoModeNameList.length;
         var _loc3_:uint = 0;
         while(_loc3_ < _loc2_)
         {
            if(param1 == autoModeNameList[_loc3_])
            {
               g.setAuto(_loc3_);
               g.inGameMenu.updateAutoList();
               break;
            }
            _loc3_++;
         }
      }
      
      public function reducePleasureLoss() : Boolean
      {
         if(g.autoModeOn && this.randomMode == this.softAuto)
         {
            return true;
         }
         return false;
      }
      
      public function resetSelfMode() : void
      {
         this.currentSelfModes = this.selfModes.slice(0);
         this.usingDeepSlow = false;
         this.usingFullSlow = false;
         this.deepthroatProgress = this.currentPenisTipPos + 0.4;
      }
      
      public function startAuto(param1:Point) : void
      {
         this.t = 0;
         this.lastPos = param1;
         if(g.autoModeOn)
         {
            switch(g.autoMode)
            {
               case 0:
                  g.him.setArmPosition(0);
                  this.randomMode = this.standardAuto;
                  this.ejaculatingModes = this.normalEjacModes;
                  break;
               case 1:
                  g.him.setArmPosition(0);
                  this.randomMode = this.softAuto;
                  this.ejaculatingModes = this.softEjacModes;
                  break;
               case 2:
                  g.him.setArmPosition(0);
                  this.randomMode = this.hardAuto;
                  this.ejaculatingModes = this.hardEjacModes;
                  break;
               case 3:
                  g.him.setArmPosition(1);
                  this.randomMode = this.handsOffAuto;
                  this.ejaculatingModes = this.selfEjacModes;
                  break;
               default:
                  g.him.setArmPosition(0);
                  this.randomMode = this.standardAuto;
                  this.ejaculatingModes = this.normalEjacModes;
            }
            this.runRandomMode();
            g.inGameMenu.updateHisArmList();
         }
         else
         {
            g.him.resetArmPosition();
         }
      }
      
      private function runRandomMode() : void
      {
         g.her.movementRelease();
         this.randomMode();
      }
      
      public function dialogueStarting() : void
      {
         if(this.randomMode == this.handsOffAuto)
         {
            this.handsOffAuto();
         }
      }
      
      public function ejaculate() : void
      {
         if(g.autoModeOn)
         {
            this.randomEjaculatingMode();
         }
      }
      
      public function doneEjaculating() : void
      {
         this.nextModeTimer = Math.floor(Math.random() * 90 + 60);
      }
      
      private function standardAuto() : void
      {
         var _loc2_:uint = 0;
         this.lastMode = this.currentMode;
         var _loc1_:Array = !!g.handJobMode?this.standardHJModes:this.standardModes;
         if(g.her.isActingOff())
         {
            this.lastModeNo = 2;
            this.currentMode = _loc1_[2];
         }
         else
         {
            do
            {
               if(g.her.intro)
               {
                  _loc2_ = Math.floor(Math.random() * (_loc1_.length - 2)) + 2;
               }
               else
               {
                  _loc2_ = Math.floor(Math.random() * _loc1_.length);
               }
            }
            while(_loc2_ == this.lastModeNo);
            
            this.lastModeNo = _loc2_;
            this.currentMode = _loc1_[_loc2_];
         }
         if(this.currentMode == this.offHold)
         {
            this.nextModeTimer = Math.floor(Math.random() * 120 + 60);
         }
         else
         {
            this.nextModeTimer = Math.floor(Math.random() * 300 + 60);
         }
         this.modeRatio = 0;
         this.transitioning = true;
         this.transitionSpeed = 0.01;
      }
      
      private function hardAuto() : void
      {
         var _loc2_:uint = 0;
         this.lastMode = this.currentMode;
         var _loc1_:Array = !!g.handJobMode?this.hardHJModes:this.hardModes;
         if(g.her.isActingOff())
         {
            this.lastModeNo = 2;
            this.currentMode = _loc1_[2];
         }
         else
         {
            do
            {
               if(g.her.intro)
               {
                  _loc2_ = Math.floor(Math.random() * (_loc1_.length - 2)) + 2;
               }
               else
               {
                  _loc2_ = Math.floor(Math.random() * _loc1_.length);
               }
            }
            while(_loc2_ == this.lastModeNo);
            
            this.lastModeNo = _loc2_;
            this.currentMode = _loc1_[_loc2_];
         }
         if(this.currentMode == this.offHold)
         {
            this.nextModeTimer = Math.floor(Math.random() * 60 + 30);
         }
         else
         {
            this.nextModeTimer = Math.floor(Math.random() * 150 + 60);
         }
         this.modeRatio = 0;
         this.transitioning = true;
         this.transitionSpeed = 0.25;
      }
      
      private function softAuto() : void
      {
         var _loc2_:uint = 0;
         this.lastMode = this.currentMode;
         var _loc1_:Array = !!g.handJobMode?this.softHJModes:this.softModes;
         if(g.her.isActingOff())
         {
            this.lastModeNo = 0;
            this.currentMode = _loc1_[0];
         }
         else if(!g.handJobMode && Math.random() > 0.8)
         {
            this.singleAction = this.oneDeep;
            this.currentMode = this.offHold;
            this.waitingForSingleAction = true;
         }
         else if(!g.handJobMode && g.breathLevel > g.outOfBreathPoint)
         {
            this.currentMode = this.offHold;
         }
         else
         {
            do
            {
               _loc2_ = Math.floor(Math.random() * _loc1_.length);
            }
            while(_loc2_ == this.lastModeNo);
            
            this.lastModeNo = _loc2_;
            this.currentMode = _loc1_[_loc2_];
         }
         if(this.currentMode == this.offHold)
         {
            this.nextModeTimer = Math.floor(Math.random() * 120 + 60);
         }
         else
         {
            this.nextModeTimer = Math.floor(Math.random() * 300 + 60);
         }
         this.modeRatio = 0;
         this.transitioning = true;
         this.transitionSpeed = 0.01;
      }
      
      private function handsOffAuto() : void
      {
         var _loc2_:uint = 0;
         this.lastMode = this.currentMode;
         var _loc1_:Array = !!g.handJobMode?this.selfHJModes:this.selfModes;
         if(g.her.isActingOff())
         {
            this.lastModeNo = 0;
            this.currentMode = _loc1_[0];
         }
         else if(!g.handJobMode && Math.random() > (!!this.usingFullSlow?0.9:0.6))
         {
            if(Math.random() > 0.99)
            {
               this.singleAction = this.waggleEyebrowsWithTongue;
            }
            else
            {
               this.singleAction = this.oneDeep;
            }
            this.currentMode = this.singleActionModes[this.singleAction];
            this.waitingForSingleAction = true;
            this.lastModeNo = 1;
         }
         else if(!g.handJobMode && Math.random() > 0.8)
         {
            _loc2_ = Math.floor(Math.random() * this.singleActions.length);
            this.singleAction = this.singleActions[_loc2_];
            this.currentMode = this.singleActionModes[this.singleAction];
            this.waitingForSingleAction = true;
         }
         else if(g.breathLevel > g.outOfBreathPoint)
         {
            this.currentMode = this.offHold;
            this.lastModeNo = 0;
         }
         else
         {
            do
            {
               _loc2_ = Math.floor(Math.random() * this.currentSelfModes.length);
            }
            while(_loc2_ == this.lastModeNo);
            
            this.lastModeNo = _loc2_;
            this.currentMode = this.currentSelfModes[_loc2_];
         }
         if(this.currentMode == this.offHold)
         {
            this.nextModeTimer = Math.floor(Math.random() * 60 + 30);
         }
         else if(this.currentMode == this.tipHold || this.currentMode == this.shallowHold)
         {
            this.nextModeTimer = Math.floor(Math.random() * 150 + 30);
         }
         else
         {
            this.nextModeTimer = Math.floor(Math.random() * 300 + 60);
         }
         this.modeRatio = 0;
         this.transitioning = true;
         this.transitionSpeed = 0.01;
      }
      
      private function randomOffTilt() : void
      {
         this.offTiltTarget = Math.random() * 1.4 - 0.9;
         this.offTiltChangeTimer = Math.floor(Math.random() * 300 + 60);
      }
      
      private function randomEjaculatingMode() : void
      {
         var _loc1_:uint = 0;
         this.lastMode = this.currentMode;
         do
         {
            _loc1_ = Math.floor(Math.random() * this.ejaculatingModes.length);
         }
         while(this.ejaculatingModes[_loc1_] == this.lastEjacMode);
         
         this.currentMode = this.ejaculatingModes[_loc1_];
         this.lastEjacMode = this.currentMode;
         this.nextModeTimer = 600;
         this.modeRatio = 0;
         this.transitioning = true;
         this.transitionSpeed = 0.05;
      }
      
      public function getPos() : Point
      {
         var _loc1_:Point = null;
         var _loc2_:Point = null;
         this.t++;
         if(g.her.released && !g.handsOff)
         {
            this.lastPos.x = Math.max(0,this.lastPos.x - 0.1);
            this.lastPos.y = this.lastPos.y * 0.8;
         }
         else if(this.transitioning)
         {
            this.modeRatio = this.modeRatio + this.transitionSpeed;
            if(this.modeRatio >= 1)
            {
               if(this.waitingForSingleAction)
               {
                  this.waitingForSingleAction = false;
                  this.singleAction();
               }
               else
               {
                  this.singleActionTime = 0;
               }
               this.singleActionTimer = 0;
               this.transitioning = false;
               this.modeRatio = 1;
            }
            _loc1_ = this.lastMode().clone();
            _loc2_ = this.currentMode();
            this.lastPos.x = _loc1_.x * (1 - this.modeRatio) + _loc2_.x * this.modeRatio;
            this.lastPos.y = _loc1_.y * (1 - this.modeRatio) + _loc2_.y * this.modeRatio;
         }
         else
         {
            this.lastPos = this.currentMode().clone();
            if(this.singleActionTimer < this.singleActionTime)
            {
               this.singleActionTimer++;
            }
            else if(this.nextModeTimer > 0)
            {
               this.nextModeTimer--;
            }
            else
            {
               this.runRandomMode();
            }
         }
         this.offTilt = this.offTilt + (this.offTiltTarget - this.offTilt) * 0.2;
         if(this.offTiltChangeTimer > 0)
         {
            this.offTiltChangeTimer--;
         }
         else
         {
            this.randomOffTilt();
         }
         this.deepthroatProgress = Math.max(this.deepthroatProgress,this.lastPos.x);
         if(g.handJobMode)
         {
            this.lastPos.x = this.lastPos.x * 0.9 + 0.1;
         }
         return this.lastPos;
      }
      
      private function adjustAim() : void
      {
         var _loc1_:Number = NaN;
         if(g.her.isInMouth() || g.handJobMode)
         {
            this.currentPos.y = this.currentPos.y * 0.8;
         }
         else
         {
            _loc1_ = g.her.getPenisMouthOffset();
            if(Math.abs(_loc1_) < 5)
            {
               return;
            }
            this.currentPos.y = this.currentPos.y + Math.max(-0.5,Math.min(0.5,_loc1_ * Math.min(0.3,10 * Math.abs(this.lastPos.x - this.currentPos.x))));
         }
      }
      
      private function avoidPenisOut() : void
      {
         if(!this.transitioning)
         {
            g.her.movementRelease();
            this.currentPos.x = Math.max(-0.2,this.currentPenisTipPos - 0.2);
            this.adjustAim();
         }
      }
      
      private function sinWave(param1:Number, param2:Number, param3:Number) : Number
      {
         return param3 + (Math.sin(this.t * param1) + 1) * param2 * 0.5;
      }
      
      private function noMovement() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         this.currentPos.x = this.currentPenisTipPos - 0.2;
         this.currentPos.y = this.lastPos.y * 0.8;
         return this.currentPos;
      }
      
      private function fullSlow() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         this.currentPos.x = Math.max(this.currentPenisTipPos - 0.3,this.sinWave(0.06,this.fullLength + 0.6,this.currentPenisTipPos - 0.4));
         if(this.currentPos.x - this.lastPos.x < 0)
         {
            this.currentPos.y = this.offTilt * (Math.cos(this.t * 0.06) * 0.5 + 0.5);
         }
         else
         {
            this.adjustAim();
         }
         return this.currentPos;
      }
      
      private function fullFast() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         this.currentPos.x = this.sinWave(0.25,this.fullLength + 0.2,this.currentPenisTipPos);
         this.currentPos.y = Math.min(0.2,Math.max(-0.2,this.offTilt));
         return this.currentPos;
      }
      
      private function deepFast() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         this.currentPos.x = this.sinWave(0.5,this.fullLength - 0.1,1.25 - this.fullLength);
         this.adjustAim();
         return this.currentPos;
      }
      
      private function deepSlow() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         this.currentPos.x = this.sinWave(0.25,this.fullLength - 0.1,1.15 - this.fullLength);
         this.adjustAim();
         return this.currentPos;
      }
      
      private function shallowSlow() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         this.currentPos.x = this.sinWave(0.25,Math.min(0.5,this.fullLength - 0.3),this.currentPenisTipPos + 0.05);
         if(this.currentPos.x - this.lastPos.x < 0 && g.her.isInMouth())
         {
            this.currentPos.y = this.offTilt;
            if(this.randomMode == this.handsOffAuto)
            {
               g.her.suck();
               g.him.givePleasure(7);
            }
         }
         else
         {
            this.adjustAim();
         }
         return this.currentPos;
      }
      
      private function shallowMedium() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         this.currentPos.x = this.sinWave(0.35,this.fullLength - 0.3,this.currentPenisTipPos + 0.05);
         if(this.currentPos.x - this.lastPos.x < 0 && g.her.isInMouth())
         {
            this.currentPos.y = this.offTilt;
            if(this.randomMode == this.handsOffAuto)
            {
               g.her.suck();
               g.him.givePleasure(7);
            }
         }
         else
         {
            this.adjustAim();
         }
         return this.currentPos;
      }
      
      private function shallowFast() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         this.currentPos.x = this.sinWave(0.5,this.fullLength - 0.3,this.currentPenisTipPos + 0.05);
         this.currentPos.y = Math.min(0.2,Math.max(-0.2,this.offTilt));
         return this.currentPos;
      }
      
      private function mediumSlow() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         this.currentPos.x = Math.max(0,this.sinWave(0.05,this.fullLength + 0.1,this.currentPenisTipPos - 0.2));
         if(this.currentPos.x - this.lastPos.x < 0 && g.her.isInMouth())
         {
            this.currentPos.y = this.offTilt;
            if(this.randomMode == this.handsOffAuto)
            {
               g.her.suck();
               g.him.givePleasure(7);
            }
         }
         else
         {
            this.adjustAim();
         }
         return this.currentPos;
      }
      
      private function tipToDeepSlow() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         this.currentPos.x = Math.max(0,this.sinWave(0.08,this.fullLength - 0.1,this.currentPenisTipPos + 0.1));
         if(this.currentPos.x - this.lastPos.x < 0 && g.her.isInMouth())
         {
            this.currentPos.y = this.offTilt;
         }
         else
         {
            this.adjustAim();
         }
         return this.currentPos;
      }
      
      private function tipToDeepMedium() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         this.currentPos.x = Math.max(0,this.sinWave(0.2,this.fullLength - 0.1,this.currentPenisTipPos + 0.1));
         if(this.currentPos.x - this.lastPos.x < 0 && g.her.isInMouth())
         {
            this.currentPos.y = this.offTilt;
         }
         else
         {
            this.adjustAim();
         }
         return this.currentPos;
      }
      
      private function tipToDeepFast() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         this.currentPos.x = Math.max(0,this.sinWave(0.35,this.fullLength - 0.1,this.currentPenisTipPos + 0.1));
         this.adjustAim();
         return this.currentPos;
      }
      
      private function mediumFast() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         this.currentPos.x = this.sinWave(0.25,this.fullLength,this.currentPenisTipPos - 0.2);
         if(this.currentPos.x - this.lastPos.x < 0 && g.her.isInMouth())
         {
            this.currentPos.y = this.offTilt;
         }
         else
         {
            this.adjustAim();
         }
         return this.currentPos;
      }
      
      private function fullSlam() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         var _loc1_:Number = Math.max(this.currentPenisTipPos - 0.2,Math.min(1.25,1.5 * Math.ceil(this.t * 0.05) - this.t * 0.05 * 1.5));
         this.currentPos.x = Math.min(this.lastPos.x + 0.5,_loc1_);
         this.adjustAim();
         return this.currentPos;
      }
      
      private function hiltMove() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         if(g.her.currentPenisDistance > g.her.currentHiltDistance)
         {
            this.currentPos.x = this.lastPos.x + Math.random() * 0.04 - 0.02;
         }
         else
         {
            this.currentPos.x = Math.min(this.lastPos.x + 0.1,1.2);
         }
         if(Math.random() > 0.6)
         {
            g.her.activeHold(false);
         }
         this.adjustAim();
         return this.currentPos;
      }
      
      private function holdDown() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         if(Math.random() > 0.99)
         {
            g.her.activeHold(false);
         }
         this.currentPos.x = Math.min(this.lastPos.x + 0.1,1.2);
         this.adjustAim();
         return this.currentPos;
      }
      
      private function activeHoldDown() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         if(Math.random() > 0.6)
         {
            g.her.activeHold(false);
         }
         this.currentPos.x = 1.2;
         this.adjustAim();
         return this.currentPos;
      }
      
      private function singleDeepthroat() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         this.currentPos.x = Math.min(this.deepthroatTarget,this.lastPos.x + 0.02);
         g.him.givePleasure(5);
         if(this.currentPos.x == this.deepthroatTarget)
         {
            this.currentPos.x = this.currentPos.x + (Math.random() * 0.01 - 0.005);
            g.her.wince();
         }
         if(this.singleActionTimer / this.singleActionTime > 0.8)
         {
            this.currentMode = this.moveOff;
            g.her.lookUp();
         }
         this.adjustAim();
         return this.currentPos;
      }
      
      private function shallowHold() : Point
      {
         if(g.penisOut)
         {
            this.avoidPenisOut();
            return this.currentPos;
         }
         this.currentPos.x = this.sinWave(0.1,0.05,this.currentPenisTipPos + 0.75);
         this.adjustAim();
         return this.currentPos;
      }
      
      private function offHold() : Point
      {
         this.currentPos.x = Math.max(-0.2,this.sinWave(0.1,0.05,this.currentPenisTipPos - 0.3));
         this.currentPos.y = this.offTilt;
         if(g.her.isActingOff())
         {
            this.nextModeTimer++;
         }
         return this.currentPos;
      }
      
      private function moveOff() : Point
      {
         this.currentPos.x = Math.max(this.currentPenisTipPos - 0.2,this.lastPos.x - 0.03);
         this.currentPos.y = this.offTilt;
         return this.currentPos;
      }
      
      private function offHoldWithTongue() : Point
      {
         this.currentPos.x = Math.max(-0.2,this.sinWave(0.1,0.05,this.currentPenisTipPos - 0.3));
         this.currentPos.y = this.offTilt;
         g.her.tongueContainer.tongue.encourage();
         return this.currentPos;
      }
      
      private function tipHold() : Point
      {
         var _loc1_:Number = g.her.getPenisMouthDist() + 10;
         this.currentPos.x = this.currentPos.x - _loc1_ * 0.001;
         this.currentPos.y = this.offTilt;
         g.her.tongueContainer.tongue.encourage();
         return this.currentPos;
      }
      
      private function outsideLickBalls() : Point
      {
         if(this.transitioning)
         {
            this.currentPos.x = Math.max(this.currentPenisTipPos - 0.2,this.lastPos.x - 0.03);
         }
         else if(g.penisOut)
         {
            if(g.him.currentBalls > 0)
            {
               this.currentPos.x = Math.min(1,this.lastPos.x + 0.03);
               this.currentPos.y = this.lastPos.y + (0.8 + Math.random() * 0.2 - this.lastPos.y) * 0.1;
            }
            else
            {
               this.currentPos.x = this.sinWave(0.073,this.fullLength - 0.3,this.currentPenisTipPos + 0.3);
               this.currentPos.y = this.sinWave(0.04,0.6,-0.2);
            }
            g.her.tongueContainer.tongue.encourage();
         }
         else
         {
            this.currentPos.x = Math.max(this.currentPenisTipPos - 0.2,this.lastPos.x - 0.03);
            this.currentPos.y = this.offTilt;
            g.her.mousePressed();
         }
         return this.currentPos;
      }
      
      private function waggleEyebrows() : void
      {
         this.nextModeTimer = 0;
         this.singleActionTime = 25;
         g.her.waggleEyebrows();
      }
      
      private function waggleEyebrowsWithTongue() : void
      {
         this.nextModeTimer = 0;
         this.singleActionTime = 25;
         g.her.waggleEyebrows();
         g.her.tongueContainer.tongue.encourage(50);
      }
      
      private function oneDeep() : void
      {
         this.nextModeTimer = 0;
         this.singleActionTime = 120;
         this.deepthroatTarget = Math.random() * 0.1 + 0.1 + this.deepthroatProgress;
         if(g.her.currentPenisDistance > g.her.deepthroatDistance && !this.usingDeepSlow)
         {
            this.usingDeepSlow = true;
            this.currentSelfModes.push(this.deepSlow);
         }
         if(g.her.currentPenisDistance > g.her.currentHiltDistance && !this.usingFullSlow)
         {
            this.usingFullSlow = true;
            this.currentSelfModes.push(this.fullSlow);
         }
         this.currentMode = this.singleDeepthroat;
         g.her.lookDown();
      }
   }
}
