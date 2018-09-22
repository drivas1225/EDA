let qtree;

function setup() {
  createCanvas(400,400);

   let boundary = new Rectangle(200,200,200,200);
  qtree = new QuadTree(boundary,4);
  //console.log(qtree);

   for (let i = 0; i < 300 ; i++) {
     let p = new Point(random(width),random(height));
     qtree.insert(p);
   }



}


//noLoop();
//}

function draw() {
  if(mouseIsPressed){
    let m = new Point(mouseX,mouseY);
    qtree.insert(m);
  }
  background(100);
  qtree.show();

  stroke(0,250,0);
  rectMode(CENTER);
  let range = new Rectangle(mouseX,mouseY,107,75);
  rect(range.x,range.y,range.w*2,range.h*2);
  let points = qtree.query(range);
  //console.log(points);
   for(let p of points){
     strokeWeight(4);
     point(p.x,p.y);
   }

}

function mousePressed(){
  redraw();
}
