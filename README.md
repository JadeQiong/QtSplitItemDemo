# Qt_splitTest
A Qt demo for splitting QGraphicsItem with QGraphics's function: tofillPolgons() and tofillPolgon().  
Scenario: we already have annotations for rectangles, circles and polygons, we are going to enable an automatic merge of our shapes to count the number of items currently in the scene.

## Splitting ellipse and rectangle
When dealing with Paths, its performance is not good (0.5-1s).  
I keep trace of the eraser and calculate a segment between the starting and finishing points. Then I divide the item into a left and a right part based on the side they are.  
For an ellipse,  
![image](https://github.com/JadeQiong/Qt_splitTest/blob/main/eclipse1.png)  
For a rect,  
![image](https://github.com/JadeQiong/Qt_splitTest/blob/main/rect1.png)  
![image](https://github.com/JadeQiong/Qt_splitTest/blob/main/rect2.png)  
## Merge paths  
Merging paths is a special requirement, I used PainterPath's operator "+" to achieve it.  
Its behavior is like following figures.  
![image](https://github.com/JadeQiong/Qt_splitTest/blob/main/merge1.png)  
![image](https://github.com/JadeQiong/Qt_splitTest/blob/main/merge2.png)  
