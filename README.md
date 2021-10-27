# Qt_splitTest
a Qt demo for splitting QGraphicsItem with QGraphics's function: tofillPolgons() and tofillPolgon.  
When dealing with Paths, its performance is not good.  
I keep trace of the eraser and calculate a segment between the starting and finishing points. Then I divide the item into a left and a right part based on the side they are.
## Splitting ellipse and rectangle
![image](https://github.com/JadeQiong/Qt_splitTest/blob/main/eclipse1.png)  
![image](https://github.com/JadeQiong/Qt_splitTest/blob/main/rect1.png)  
![image](https://github.com/JadeQiong/Qt_splitTest/blob/main/rect2.png)  
## Merge paths
![image](https://github.com/JadeQiong/Qt_splitTest/blob/main/merge1.png)  
![image](https://github.com/JadeQiong/Qt_splitTest/blob/main/merge2.png)  
