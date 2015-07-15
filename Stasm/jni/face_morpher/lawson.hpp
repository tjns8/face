/*
 * lawson.hpp
 *
 *  Created on: 2015-7-14
 *      Author: bestlove
 */
namespace lawson {
class Face;
class EdgeFace {
	//包含边的信息
public:
	Face* face;
	int otherPointIndex;
	EdgeFace(Face* f, int opi) {
		this->face = f;
		this->otherPointIndex = opi;
	}
	float* getOtherPoint() {
		return face->point[otherPointIndex];
	}
	void set(EdgeFace* ef) {
		this->face = ef->face;
		this->otherPointIndex = ef->otherPointIndex;
	}
	void updateOtherFaceForMyEdgeFaceInfo(int i, Face* f) {
		//更新目标face里的对应于本三角形的edgeface属性
		int index = otherPointIndex + 1 >= 3 ? 0 : otherPointIndex + 1;
		if (face == 0){
			return;
		}
		if (face->edgeFaces[index] == 0) {
			face->edgeFaces[index] = new EdgeFace(f, i);
		} else {
			face->edgeFaces[index]->face = f;
			face->edgeFaces[index]->otherPointIndex = i;
		}
	}
};
class Face {
	float x, y, r;
	float squareSum(float a, float b) {
		return a * a + b * b;
	}
	float determinant3order(float a1, float b1, float c1, float a2, float b2,
			float c2, float a3, float b3, float c3) {
		return a1 * b2 * c3 + b1 * c2 * a3 + c1 * a2 * b3 - a3 * b2 * c1
				- b3 * c2 * a1 - c3 * a2 * b1;
	}
	float len(float ax, float ay, float bx, float by) {
		float dx = bx - ax;
		float dy = by - ay;
		return sqrtf(squareSum(dx, dy));
	}
	void createCircle() {
		float p0 = squareSum(point[0][0], point[0][1]);
		float p1 = squareSum(point[1][0], point[1][1]);
		float p2 = squareSum(point[2][0], point[2][1]);
		float xu = determinant3order(p0, point[0][1], 1, p1, point[1][1], 1, p2,
				point[2][1], 1);
		float yu = determinant3order(point[0][0], p0, 1, point[1][0], p1, 1,
				point[2][0], p2, 1);
		float d = 2
				* determinant3order(point[0][0], point[0][1], 1, point[1][0],
						point[1][1], 1, point[2][0], point[2][1], 1);
		x = xu / d;
		y = yu / d;
		r = len(x, y, point[0][0], point[0][1]);
	}
	//空外接圆检测
	bool isInCircle(float px, float py) {
		if (r < 0)
			createCircle();
		float pr = len(x, y, px, py);
		if (pr <= r)
			return true;
		return false;
	}
	bool isInCircle(float* ps) {
		return isInCircle(ps[0], ps[1]);
	}
	//三角形区域判别式
	bool isInTrangle(float px, float py, float ax, float ay, float bx, float by,
			float cx, float cy) {
		float v0x = cx - ax;
		float v0y = cy - ay;
		float v1x = bx - ax;
		float v1y = by - ay;
		float v2x = px - ax;
		float v2y = py - ay;
		float v00 = v0x * v0x + v0y * v0y;
		float v11 = v1x * v1x + v1y * v1y;
		float v01 = v0x * v1x + v0y * v1y;
		float gdp = v00 * v11 - v01 * v01;
		float v02 = v0x * v2x + v0y * v2y;
		float v12 = v1x * v2x + v1y * v2y;
		float u = (v11 * v02 + v01 * v12) / gdp;
		float v = (v00 * v12 - v01 * v02) / gdp;
		if (u < 0)
			return false;
		if (v < 0)
			return false;
		if (u + v > 1)
			return false;
		return true;
	}
public:
	EdgeFace* edgeFaces[3]; //i (i,i+1)
	Face* next;
	//point代表三角形三个点的位置索引
	int pointindex[3];
	float point[3][2];
	Face() {
		next = 0;
		r = -1;
		for (int i = 0; i < 3; i++) {
			pointindex[i] = 0;
			edgeFaces[i] = 0;
		}
	}
	void clearCircle() {
		r = -1;
	}
	bool isInTriangle(float* ps) {
		float px = ps[0];
		float py = ps[1];
		return isInTrangle(px, py, point[0][0], point[0][1], point[1][0],
				point[1][1], point[2][0], point[2][1]);
	}
	void setPoint0(float x, float y) {
		point[0][0] = x;
		point[0][1] = y;
	}
	void setPoint1(float x, float y) {
		point[1][0] = x;
		point[1][1] = y;
	}
	void setPoint2(float x, float y) {
		point[2][0] = x;
		point[2][1] = y;
	}
	float* getPoint(int i) {
		return point[i];
	}
	void setPoint(int i, float* p) {
		point[i][0] = p[0];
		point[i][1] = p[1];
	}

	void addInEnd(Face* f) {
		if (next == 0) {
			next = f;
		} else {
			next->addInEnd(f);
		}
	}
	void lop() {
		//前提条件，三点顺序在所有三角形中都是顺时针
		//提供的两个三角形必须相邻，且index必须已经被赋值
		for (int i = 0; i < 3; i++) {
			if (edgeFaces[i] == 0)
				continue;
			if (!isInCircle(edgeFaces[i]->getOtherPoint()))
				continue;
			//需要进行变换
			Face* b = edgeFaces[i]->face;
			int bo = edgeFaces[i]->otherPointIndex; //b中的外点
			Face* a = this;
			int ao = i - 1 >= 0 ? i - 1 : 2; //a中的外点
			int la1 = i; //a共线端点
			int la2 = i + 1 >= 3 ? 0 : i + 1; //a共线端点
			int lb1 = bo + 1 >= 3 ? 0 : bo + 1; //b共线端点
			int lb2 = lb1 + 1 >= 3 ? 0 : lb1 + 1; //b共线端点
			a->setPoint(la2, b->getPoint(bo));
			b->setPoint(lb2, a->getPoint(ao));
			a->edgeFaces[la1]->set(b->edgeFaces[lb2]);
			a->edgeFaces[la1]->updateOtherFaceForMyEdgeFaceInfo(ao, a);
			b->edgeFaces[lb1]->set(a->edgeFaces[la2]);
			b->edgeFaces[lb1]->updateOtherFaceForMyEdgeFaceInfo(bo, b);
			a->edgeFaces[la2]->face = b;
			a->edgeFaces[la2]->otherPointIndex = lb1;
			b->edgeFaces[lb2]->face = a;
			b->edgeFaces[lb2]->otherPointIndex = la1;
			//变换结束后，原三角形的la1不变
			i = 0; //重新处理，直到所有相邻三角形都满足lop
			b->lop();
		}
	}
};
class Delaunay {
private:
	float* points;
	int pointslen;
	Face* faceList;
	void insert(float* p) {
		Face* fp = faceList;
		while (fp != 0) {
			if (!fp->isInTriangle(p)) {
				fp = fp->next;
				continue;
			}
			//插入点拆分三角形
			Face* a = fp;
			Face* b = new Face();
			Face* c = new Face();
			c->next = a->next;
			b->next = c;
			a->next = b;
			b->setPoint(0, a->getPoint(0));
			b->setPoint(1, a->getPoint(1));
			b->setPoint(2, p);
			c->setPoint(1, a->getPoint(1));
			c->setPoint(2, a->getPoint(2));
			c->setPoint(0, p);
			a->setPoint(1, p);
			//修正三角形临边信息
			b->edgeFaces[0] = a->edgeFaces[0];
			c->edgeFaces[1] = a->edgeFaces[1];
			a->edgeFaces[0] = new EdgeFace(b, 1);
			a->edgeFaces[1] = new EdgeFace(c, 1);
			b->edgeFaces[1] = new EdgeFace(c, 2);
			b->edgeFaces[2] = new EdgeFace(a, 2);
			c->edgeFaces[2] = new EdgeFace(a, 0);
			c->edgeFaces[0] = new EdgeFace(b, 0);
			a->clearCircle();
			a->edgeFaces[2]->updateOtherFaceForMyEdgeFaceInfo(1, a);
			b->edgeFaces[0]->updateOtherFaceForMyEdgeFaceInfo(2, b);
			c->edgeFaces[1]->updateOtherFaceForMyEdgeFaceInfo(0, c);
			a->lop();
			b->lop();
			c->lop();
			break;
		}
		if(fp==0)return;
	}

public:
	Delaunay(float* points, int pointslen) {
		this->points = points;
		this->pointslen = pointslen;
		//查找点集最大最小值
		float xa, xi, ya, yi;
		xa = points[0];
		xi = points[0];
		ya = points[1];
		yi = points[1];
		for (int i = 0; i < pointslen; i++) {
			float x = points[i * 2];
			float y = points[i * 2 + 1];
			xa = xa < x ? x : xa;
			xi = xi > x ? x : xi;
			ya = ya < y ? y : ya;
			yi = yi > y ? y : yi;
		}
		//构造初始三角网
		Face* fistA = new Face();
		Face* fistB = new Face();
		faceList = fistA;
		fistA->next = fistB;
		fistA->setPoint0(xi, yi); //-1
		fistA->setPoint1(xa, yi); //-2
		fistA->setPoint2(xi, ya); //-3
		fistB->setPoint0(xa, yi); //-2
		fistB->setPoint1(xi, ya); //-3
		fistB->setPoint2(xa, ya); //-4
		fistB->next = 0;
		fistA->pointindex[0] = -1;
		fistA->pointindex[1] = -2;
		fistA->pointindex[2] = -3;
		fistB->pointindex[0] = -2;
		fistB->pointindex[1] = -3;
		fistB->pointindex[2] = -4;
		//初始化lop条件
		fistA->edgeFaces[1] = new EdgeFace(fistB, 2);
		fistB->edgeFaces[0] = new EdgeFace(fistA, 0);
		fistA->lop();
		fistB->lop();
		for(int i=0;i<pointslen;i++){
			float* fp=&points[i*2];
			insert(fp);
		}
	}
};

}

