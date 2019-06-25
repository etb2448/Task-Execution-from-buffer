#include<iostream>
using namespace std;

// Temps d'arriver des données
class AvantExecution // Class Avant Execution qui cree un tableau dynamique et recupere l'instant d'arriver de chaque donnée qui rentre en execution
{
public:
	AvantExecution(int cap);
	AvantExecution(){}
	void ReceptionAvant(int Entree, int pos);
	int getValue(int i);
	//void afficher();
private:
	int *E;
	int Entree;
	int Tache;
	class BUFFER friend;
};
AvantExecution::AvantExecution(int cap) // Constructeur
{
	E = new int[cap];
	Tache = cap;
}
void AvantExecution::ReceptionAvant(int Entree, int pos) // fonction qui permet  de recuperer une donne et l'inserer dans un tableau a une position defini
{
	E[pos] = Entree;
}
int AvantExecution::getValue(int i) // fonction permettant de recuperer une valeur du tableau a une position definis
{
	return E[i];
}
// Durée d'execution des données
class ApresExecution : public AvantExecution //Class Apres Execution qui cree un tableau dynamique et recuperer la durre de chaque donée en execution 
{
public:
	ApresExecution(int cap);
	ApresExecution() {};
	void ApresReception(int Sortie,int pos);
	int getValue(int i);
private:
	int *S;
	int Tache;
	class BUFFER friend;
};
ApresExecution::ApresExecution(int cap) // CONSTRUCTEUR
{
	S = new int[cap];
	Tache = cap;
}
void ApresExecution::ApresReception(int Sortie, int pos) //fonction qui permet a son appel de recuperer une donne et le mettre dans un tableau defini
{
	S[pos] = Sortie;
}
int ApresExecution::getValue(int i) // fonction permettant de recuperer une valeur du tableau a partir d'une autre classe
{
	return S[i];
}

//BUFFER
class BUFFER: public ApresExecution // Class BUFFER contennat une fonction qui fais la similation de l'execution des taches au niveau d'un serveur a l'aide de deux autres class defini ci haut
{
public:
	BUFFER(int cap);
	bool estVide();
	bool estPlein();
	void enfiler(int n);
	void defiler();
	void Execution(int Tache);
	//void afficher();
private:
	int *F;
	int *En;
	int *S0;
	int capacite;
	int taille;
	int Tache;
	int t;
	int q;
};
BUFFER::BUFFER(int cap) //Constructeur
{
	F = new int[cap];
	capacite = cap;
	taille = 0;
	t = q = 0;

}
bool BUFFER::estVide() // Verifie si le BUFFER est vide
{
	if(q == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool BUFFER::estPlein() // Verifie si le BUFFER est plein
{
	if (q == capacite)
	{
		return true;
	}
	else
		return false;
}
void BUFFER::enfiler(int n) // fonction pour ajouter un element dans une file 
{
	if (q >= capacite)
	{
		//cout << "File pleine" << endl;
	}
	else
	{
		F[q] = n;
		q++;
		taille++;
	}
}
void BUFFER::defiler() // Fonction permettant de Supprimer un element dans une file 
{
	for (int i = 0; i < q; i++)
	{
		F[i] = F[i + 1];
	}
	q--;
	taille--;
}
void BUFFER::Execution(int Tache) // Fonction permettant de faire la simulation 
{
	int En, Sr; 
	ApresExecution Apres(Tache);
	AvantExecution Avant(Tache);
	for (int i = 0; i < Tache; i++) // Recuperer les donnees dans le fichier et le mettre respectivement dans le Tableau des Entrees et dans le tableau des Sortie
	{
		cin >> En;
		Avant.ReceptionAvant(En, i);
		cin >> Sr;
		Apres.ApresReception(Sr, i);

	}
	int i = 0;
	int prochaineSortie = Apres.getValue(i); // Recuperer la première valeur du Tableau Sortie et le mettre dans une varible 
	while (i < Tache) // Tant qu'il ya encore de tâche a faire 
	{
		if (estVide()) // Si le BUffer est vide 
		{
			enfiler(Avant.getValue(i)); // enfile 
			cout << Avant.getValue(i) << " "; // et affiche
			i++;

		}
		while (!estPlein()) // Tant que le buffer n'est pas plein 
		{
			// Verifie si la donnée qui vient pour l'execution arrive avant ou apres la fin de l'exécution en cours
			if (Avant.getValue(i) >= prochaineSortie) // Si la donnée arrive après la fin de l'execution 
			{
				enfiler(Avant.getValue(i)); // Enfile la valeur de l'instant à laquelle la donnée est arrivée
				cout << Avant.getValue(i) << " ";
				prochaineSortie = Avant.getValue(i) + Apres.getValue(i); // Incremente la prochaine sortie en faisant la somme de l'arriver de la donne a sa fin d'execution
			}
			else //Sinon
			{
				enfiler(prochaineSortie); // Enfile la duree de la derniere execution qui sera l'instant à laquelle la prochaine donnée va commencer son exécution
				cout << prochaineSortie << " "; // Et affiche la 
				prochaineSortie += Apres.getValue(i); // Puis incremente cette duree par la duree  de la valeur qui vient dans le buffer
			}
			
			i++;
		}
		if (Tache > 1) // Verifie s'il ya plus qu'une tâche
		{
			if (Avant.getValue(i) >= F[1] && Avant.getValue(i) > Avant.getValue(i-1)) // Verifie si la nouvelle donnée vient avant ou apres la fin de l'execution en cours et s'assure que la donne ne vient pas au moment que la donne en  attente
			{
				defiler(); 
				if (Avant.getValue(i) >= prochaineSortie) // Si la donnee arrive apres ou au moment meme de la fin de l'execution en cours
				{
					enfiler(Avant.getValue(i)); 
					cout << Avant.getValue(i) << " "; // Affiche son temps d'arriver
					prochaineSortie = Avant.getValue(i) + Apres.getValue(i);
				}
				else // Sinon affiche le temps à laquelle l'execution a pris fin
				{
					enfiler(prochaineSortie);
					cout << prochaineSortie << " "; // Affiche le temps de la fin d'execution
					prochaineSortie += Apres.getValue(i);
				}
			}
			else // Sinon le BUFFER est encore  plein et il n'ya pas de place donc la tache ne sera pas exécuter 
			{
				cout << -1 << " "; // On affiche donc -1 pour identifier les tâches non exécuter
			}
			i++;
		}
	}
}
int main()
{
	int capacite;
	int Tache;
	cin >> capacite;
	cin >> Tache;
	BUFFER buffer(capacite);
	buffer.Execution(Tache);
	return 0;
}
