/*
 * spatial.hpp
 *
 *  Created on: 2015-6-29
 *      Author: bestlove
 */
#include "stdio.h"
#include "math.h"
#include "setjmp.h"
#include "init.hpp"
#include <opencv2/imgproc.hpp>

//extern "C"{
//#include "qhull/qset.h"
//#include "qhull/libqhull.h"

//extern int qh_new_qhull(int dim, int numpoints, coordT *points, boolT ismalloc,char *qhull_cmd, FILE *outfile, FILE *errfile);
//extern int qh_pointid(pointT *point) ;
//}
extern "C"{
namespace spatial {

class Delaunay {
private:
	double (*transforms)[3][3];
	float* points;
	int pointslen;
public:
	int simpliceslen; //三角网格数量
//分解的三角网格数组	Nx3	元素为点下标
	int (*simplices)[3]; //qhull.pyx -1750	Delaunay _update()
	//			-2242	ConvexHull _update()
	//构造阶段位于542~683 facets返回值
	//
	//对应函数542~683
	Delaunay(float* points, int pointslen) { //points只包含一张脸的点
		//对应于__init()__
		aprintf("delaunay create");
//		if(true)return;
		this->points = points;
		this->pointslen = pointslen;
		double* dpoints = new double[pointslen];
		for (int i = 0; i < pointslen; i++) {
			dpoints[i] = points[i];
		}
		//内存存储器是一个可用来存储诸如序列，轮廓，图形,子划分等动态增长数据结构的底层结构。
		char* opt ="Qbb Qc Qz Qx Qt";
		aprintf("call qh new qhull start");
		qh_new_qhull(2, pointslen, dpoints, 0, opt, stdout, stderr);
		aprintf("call qh new qhull end");
		//对应simplices生成
		//607
		facetT* facet = qh_qh.facet_list;
		int j = 0;
		while (facet && facet->next) {
			j++;
			facet = facet->next;
		};
		int (*facets)[3] = new int[j][3];
		transforms = new double[j][3][3];
		simpliceslen = j;
		j = 0;
		facet = qh_qh.facet_list;
		while (facet && facet->next) {
			double* vertexa = (double*) (facet->vertices->e[0].p);
			int ipointa = qh_pointid(vertexa);
			facets[j][1] = ipointa;
			double* vertexb = (double*) (facet->vertices->e[1].p);
			int ipointb = qh_pointid(vertexb);
			facets[j][0] = ipointb;
			double* vertexc = (double*) (facet->vertices->e[2].p);
			int ipointc = qh_pointid(vertexc);
			facets[j][2] = ipointc;
			makeTransforms(transforms[j], vertexa, vertexb, vertexc);
			j += 1;
			facet = facet->next;
		}
		simplices = facets;
		aprintf("delaunay create");
	}

	void makeTransforms(double transform[3][3], double* a, double* b,
			double* c) {
		//a,b,c [2]
		//transform[3][3]	{V0x,V1x,Ax}
		//					{V0y,V1y,Ay}
		//					{d00,d01,d11}
		transform[0][0] = c[0] - a[0];
		transform[1][0] = c[1] - a[1];
		transform[0][1] = b[0] - a[0];
		transform[1][1] = b[1] - a[1];
		transform[0][2] = a[0];
		transform[1][2] = a[1];
		transform[2][0] = transform[0][0] * transform[0][0]
				+ transform[1][0] * transform[1][0];
		transform[2][1] = transform[0][0] * transform[0][1]
				+ transform[1][0] * transform[1][1];
		transform[2][2] = transform[0][1] * transform[0][1]
				+ transform[1][1] * transform[1][1];
	}

	//1929~2008
	//	->1342~1451
	//	->1240~1340
	//		->1105~1123
	//		->1182~1238
	//	->1182~1238
	int* find_simplex(int** roi_coords, int roi_len) {
		//返回roi_coords上每个点所所属的三角形的下标
		//roi_coords Nx2
		aprintf("find simplex");
//		if(true)return 0;
		int* out = new int[roi_len];
		for (int i = 0; i < roi_len; i++) { //遍历输入点
			int* roi_coord = roi_coords[i];
			for (int j = 0; j < simpliceslen; j++) { //遍历索引
				double (*transform)[3] = transforms[j];
				double v2[2] = { roi_coord[0] - transform[0][2], roi_coord[1]
						- transform[1][2] };
				double d02 = transform[0][0] * v2[0] + transform[1][0] * v2[1];
				double d12 = transform[0][1] * v2[0] + transform[1][1] * v2[1];
				double inv = 1
						/ (transform[2][0] * transform[2][2]
								- transform[2][1] * transform[2][1]);
				double u = (transform[2][2] * d02 - transform[2][1] * d12)
						* inv;
				if (u < 0 || u > 1)
					continue;
				double v = (transform[2][0] * d12 - transform[2][1] * d02)
						* inv;
				if (v < 0 || v > 1)
					continue;
				if (u + v > 1)
					continue;
				out[i] = j;
				break;
			};
		}
		return out;
	}

}
;
}

}
