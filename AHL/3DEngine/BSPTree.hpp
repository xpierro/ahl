#ifndef _BSP_T_H_
#define _BSP_T_H_
#include "Plane.hpp"
#include <vector>
#include "Polygon.hpp"
#include "BoundingBox.hpp"

using namespace std;
class BSPTree {
private :
	Plane *partition;
    vector<Polygon*> polygons;
	BSPTree *front;
	BSPTree *back; // TODO : renommer en behind
	// La bounding box (orientée)
	BoundingBox *bBox;
	// Squelette de PVS
	/**
	 * Idée : chaque noeud a un identifiant unique, à la fin de la génération
	 * 		  du BSP on va chercher a trouver tous les identifiants de noeuds visibles
	 * 		  à partir de chaque noeud.
	 * Question : Mais comment savoir ca ?
	 * Réponses : Ray tracing, portal finding
	 * Question : Qu'est-ce qu'un portail ? CONCRETEMENT ?
	 * Réponse : Un polygone présent dans deux noeuds ?
	 * Problème : Ne gère pas la transparence
	 */
	static int idCounter;
	int id;
	vector<int*> visibleNodes;

public :
	BSPTree() {};
	static BSPTree* buildBSPTree(vector<Polygon*> polygons);
	void draw(MathVector&, MathVector&, bool);
};

#endif
