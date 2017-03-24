var myObstacles = [];
var finishedMoving; //boolean
var gameFinished;
var feedFlag;
var gameSpeed = 75;
var score = 0;
var scoreMultiplier = 1000 / gameSpeed;
scoreMultiplier = Math.round(scoreMultiplier);
function startGame() {
	finishedMoving = false;
    myGameArea.start();
	myScore = new component("18px", "Consolas", "white", 340, 20, "text");
    myGamePiece = new component(30, 30, "white", myGameArea.canvas.width/2, myGameArea.canvas.height/2, "not text", 5);
}

var myGameArea = {
    canvas : document.createElement("canvas"),
    start : function() {
		feedFlag = true;
        this.canvas.width = 480;
        this.canvas.height = 270;
        this.context = this.canvas.getContext("2d");
        document.body.insertBefore(this.canvas, document.body.childNodes[0]);
		this.interval = setInterval(updateGameArea, 20);
		window.addEventListener('keydown', function (e) {
			myGameArea.keys = (myGameArea.keys || []);
			myGameArea.keys[e.keyCode] = true;
			//disable scroll down
			if([32, 37, 38, 39, 40].indexOf(e.keyCode) > -1) {
				e.preventDefault();
			}
		})
		window.addEventListener('keyup', function (e) {
            myGameArea.keys[e.keyCode] = false;
        })
		gameFinished = false;
		this.frameNo = 0;
    },
	clear : function() {
		this.context.clearRect(0, 0, this.canvas.width, this.canvas.height);
	}
}



function everyinterval(n) {
	if ((myGameArea.frameNo / n) % 1 == 0)
		return true;
	return false;
}

function component(width, height, color, x, y, type, border = -1) {
	this.type = type;
    this.width = width;
    this.height = height;
    this.x = x;
    this.y = y; 
	this.speedX = 0;
	this.speedY = 0;
	this.update = function() {
		ctx = myGameArea.context;
		if (type == "text"){
			ctx.font = this.width + " " + this.height;
			ctx.fillStyle = color;
			ctx.fillText(this.text, this.x, this.y);
		}
		else {
			ctx.fillStyle = color;
			ctx.fillRect(this.x, this.y, this.width, this.height);
			
			ctx.fillStyle = "red";
			if (border != -1)
				ctx.fillRect(this.x + border, this.y + border, this.width - (border*2), this.height - (border*2));
		}
	}
	this.newPos = function() {
		this.x += this.speedX;
		this.y += this.speedY;
	}
	this.collisionCheck = function(otherComp) {
		var thisLeft = this.x;
		var thisRight = this.x + (this.width);
		var thisTop = this.y;
		var thisBott = this.y + (this.height);
		var otherLeft = otherComp.x;
		var otherRight = otherComp.x + (otherComp.width);
		var otherTop = otherComp.y;
		var otherBott = otherComp.y + (otherComp.height);
		var collision = true;
		if ((thisBott < otherTop) || (thisTop > otherBott) || (thisRight < otherLeft) || (thisLeft > otherRight)){
			collision = false;
		}
		return collision;	
	}
}

function updateGameArea() {
	var resetPushed = false;
	if (!gameFinished && myGamePiece.x < myGameArea.canvas.width - 31.1 && myGamePiece.y < myGameArea.canvas.height - 31.1 && myGamePiece.x >= 5 && myGamePiece.y >= 2.5) {
		
		//left
		if (myGameArea.keys && myGameArea.keys[37] && myGamePiece.speedX > -3){
			movePiece.moveleft();
		}
		//right
		if (myGameArea.keys && myGameArea.keys[39] && myGamePiece.speedX < 1) {
			movePiece.moveright(); 
		}
		//up
		if (myGameArea.keys && myGameArea.keys[38] && myGamePiece.speedY > -3) {
			movePiece.moveup();
		}
		//down
		if (myGameArea.keys && myGameArea.keys[40] && myGamePiece.speedY < 3) {
			movePiece.movedown();
		}
		
		//stop
		if (myGameArea.keys && myGameArea.keys[83]) {
			movePiece.stop();
		}
		
		
		
		
		myGameArea.clear();
		myGameArea.frameNo++;
		score += scoreMultiplier;
		myScore.text="SCORE: " + score;
		myScore.update();
		if (myGameArea.frameNo == 1 || everyinterval(gameSpeed)){
			updateObstacles();
		}
		for (var i = 0; i < myObstacles.length;i++){
			myObstacles[i].x += - 1;
			if (myObstacles[i].x < 0)
				myObstacles.splice(i, 1);
			myObstacles[i].update();
		}
		myGamePiece.newPos();
		myGamePiece.update();
	}
	//hit a side
	else if (!finishedMoving){
		report("Hit a side. Score: " + score.toString());
		gameFinished = true;
		finishedMoving = true;
		movePiece.stop();
		score = 0;
	}
	//obstacle stop
	var x, y;
	for (var i = 0; i < myObstacles.length; i++){
		if (myGamePiece.collisionCheck(myObstacles[i]) && !finishedMoving) {
			report("Hit an obstacle. Score: " + score.toString());
			finishedMoving = true;
			gameFinished = true;
			movePiece.stop();
			score = 0;
				
		}
	}
	//reset
	if (myGameArea.keys && myGameArea.keys[82] && !resetPushed) {
		movePiece.reset();
	}
	if (!(myGameArea.keys && myGameArea.keys[82])){
		resetPushed = false;
	}
	
}

var movePiece = {
	moveup : function() {
		myGamePiece.speedY -= 1; 
	},

	movedown : function() {
		myGamePiece.speedY += 1; 
	},

	moveleft : function() {
		myGamePiece.speedX -= 1;
	},

	moveright : function() {
		myGamePiece.speedX += 1;
	},
	
	stop : function() {
		myGamePiece.speedX = 0;
		myGamePiece.speedY = 0;
	},
	
	speedUp : function() {
		if (gameSpeed > 50)
			gameSpeed -= 25;
		scoreMultiplier = 1000 / gameSpeed;
		scoreMultiplier = Math.round(scoreMultiplier);
	},
	
	slowDown : function() {
		if (gameSpeed < 400)
			gameSpeed += 25;
		scoreMultiplier = 1000 / gameSpeed;
		scoreMultiplier = Math.round(scoreMultiplier);
	},
	
	reset : function() {
		resetPushed = true;
		gameFinished = false;
		myGameArea.keys[82] = false;
		finishedMoving = false;
		myGamePiece.x = myGameArea.canvas.width/2; 
		myGamePiece.y = myGameArea.canvas.height/2;
		for (i = 0; i < myObstacles.length; i++){
			myObstacles.pop();
		}
		myObstacles.length = 0;
		movePiece.stop();
		score = 0;
	},
	memeFeed : function() {
		if (feedFlag)
			feedFlag = false;
		else
			feedFlag = true;
	}
}

function report(message) {
	if (feedFlag) {
		var para = document.createElement("p");
		var node = document.createTextNode(message);
		para.appendChild(node);
			
		var element = document.getElementById("div1");
		element.insertBefore(para, element.childNodes[0]);
	}
}

function updateObstacles () {
	//type 1 is random place, 1 semi random size
	//type 2 is two with semi random gap between them
	var type = Math.random() * 100;
	var x = myGameArea.canvas.width;
	var size;
	var y;

	//type 1
	if (type > 50) {
		size = (((Math.random() * 1000) % 80) + 100); 
		y = ((Math.random() * 1000) % myGameArea.canvas.height);
		if ((y - size) > -40)
			y -= size;
		else
			y += (size /2)
		myObstacles.push(new component(10, size, "blue", x, y, "not text", 3));
	}
	//type 2
	else {
		size = (((Math.random() * 1000) % 50) + 50);
		y = ((Math.random() * 1000) % (myGameArea.canvas.height / 2)) - size; 
		myObstacles.push(new component(10, size, "blue", x, y, "not text", 3));
		
		size = (((Math.random() * 1000) % 35) + 50);
		y = (myGameArea.canvas.height / 2) + (((Math.random() * 1000) % (myGameArea.canvas.height / 2)) - (size / 2));
		myObstacles.push(new component(10, size, "blue", x, y, "not text", 3));	
	}
}