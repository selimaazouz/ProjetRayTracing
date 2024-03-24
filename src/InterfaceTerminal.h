#ifndef INTERFACE_TERMINAL_H_INCLUDED
#define INTERFACE_TERMINAL_H_INCLUDED
#include <ncurses.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <thread>
#include <stdexcept>
#include <memory>
#include "sphere.hpp"
#include "Mobile_Sphere.h"
#include "materiau.h"
#include "MoteurDeRendu.h"
#include "vecteur3.h"

#include "rt.h"
namespace interfaceTerminal {
    class InterfaceTerminal {

        public:
            /* Constructeur */
            InterfaceTerminal(sf::RenderWindow&, MoteurRendu&);

            /* Initialise le terminal ncurses et démarre le thread */
            void initialiser();

            /* Joindre le thread */
            void fermer();

            /* Renvoie true si l'utilisateur demande de fermer l'application */
            bool estTempsDeFermer();

        private:
            std::thread threadInterface;
            bool tempsDeFermer = false;
            sf::RenderWindow& fenetreSFML;
            MoteurRendu& moteurRT;
            WINDOW* fenetreBarreDeProgression, *fenetreEnTete, *fenetreSaisie, *fenetreOpt;

            /* Définit la fenêtre principale du terminal */
            void principalNcurses();

            /* Met à jour la barre de progression si le moteur est en cours d'exécution */
            void mettreAJourBarreDeProgression();

            /* Crée l'en-tête */
            void initialiserFenetreEnTete();

            /* Affiche les options pour l'utilisateur */
            void initialiserFenetreOpt();

            /* Obtient une entrée de l'utilisateur sous forme de chaîne */
            std::string obtenirParametre(int ligne);

            /* Crée une fenêtre de nom de fichier et obtient une entrée */
            std::string obtenirNomFichier();

            /* Obtient une entrée en double */
            double obtenirDoubleParametre(int ligne);

            /* Obtient une entrée en entier */
            int obtenirEntierParametre(int ligne);

            /* Obtient une entrée sous forme de point (par exemple Vec3 ou Couleur) */
            point obtenirpointParametre(int ligne);

            /* Obtient un fichier XML et initialise le moteur de ray tracing à partir de celui-ci */
            void recupererXML();

            /* Enregistre le moteur de ray tracing actuel dans un fichier XML pouvant être rechargé */
            void enregistrerXML();

            /* Enregistre la scène rendue dans une image */
            void enregistrerImage();

            /* Crée une scène avec tous les paramètres saisis par l'utilisateur */
            void nouvelleScene();

            /* Crée un pointeur de sphère mobile avec les entrées de l'utilisateur */
            std::shared_ptr<Mobile_Sphere> creerSphereMobile(int ligne);

            /* Crée un pointeur de sphère avec les entrées de l'utilisateur */
            std::shared_ptr<SphereObject> creerSphere(int ligne);

            /* Crée un pointeur de matériau avec les entrées de l'utilisateur */
            std::shared_ptr<materiau> selectionnerMateriau(int ligne);

    };

    InterfaceTerminal::InterfaceTerminal(sf::RenderWindow& fenetre, MoteurRendu& moteur) : fenetreSFML(fenetre), moteurRT(moteur), fenetreBarreDeProgression(), fenetreEnTete(), fenetreOpt() {}

    void InterfaceTerminal::initialiser() {
        threadInterface = std::thread(&InterfaceTerminal::principalNcurses, this);
    }

    void InterfaceTerminal::fermer() {
        threadInterface.join();
    }

    bool InterfaceTerminal::estTempsDeFermer() { return tempsDeFermer; }

    void InterfaceTerminal::principalNcurses() {
        initscr();
        erase();
        cbreak();
        noecho();
        auto hauteur = 7;
        auto largeur = 54;
        auto debutx = 0;
        auto debuty = 0;
        fenetreEnTete = newwin(hauteur, largeur, debuty, debutx);
        initialiserFenetreEnTete();

        hauteur = 0;
        largeur = 20;
        debutx = 0;
        debuty = (LINES -1);
        fenetreSaisie = newwin(hauteur, largeur, debuty, debutx);

        hauteur = 30;
        largeur = 61;
        debutx = 0;
        debuty = 10;
        fenetreOpt = newwin(hauteur, largeur, debuty, debutx);

        hauteur = 2;
        largeur = 61;
        debutx = 0;
        debuty = (LINES - 2);
        fenetreBarreDeProgression = newwin(hauteur, largeur, debuty, debutx);

        sf::Sprite sprite(moteurRT.obtenirTexture());

        while(!tempsDeFermer) {
            if (moteurRT.estEnTravail()){
                mettreAJourBarreDeProgression();
            }
            else {
                wclear(fenetreBarreDeProgression);
                wrefresh(fenetreBarreDeProgression);
                initialiserFenetreOpt();
            }
        };
        endwin();
    }

    void InterfaceTerminal::mettreAJourBarreDeProgression() {
        double avancement = (double) (moteurRT.obtenirHauteurImage() - moteurRT.obtenirLignesRestantes()) / moteurRT.obtenirHauteurImage() * 100.0;
        auto finTemps = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = finTemps - moteurRT.obtenirTempsDebutTravail();

        double tempsRestant = (diff.count() / (moteurRT.obtenirHauteurImage() - moteurRT.obtenirLignesRestantes())) * moteurRT.obtenirLignesRestantes();

        werase(fenetreBarreDeProgression);
        wmove(fenetreBarreDeProgression, 0, 0);
        wprintw(fenetreBarreDeProgression, "[Temps écoulé %7.1lf s]  [Temps restant %7.1lf s]\n", diff, tempsRestant);
        wprintw(fenetreBarreDeProgression, "[");
        for (auto i = 2; i <= avancement; i += 2){
            wprintw(fenetreBarreDeProgression, "#");
        }
        mvwprintw(fenetreBarreDeProgression, 1, 51, "] %5.1lf %%", avancement);
        wrefresh(fenetreBarreDeProgression);
    }

    void InterfaceTerminal::initialiserFenetreEnTete() {
        werase(fenetreEnTete);
        wmove(fenetreEnTete, 0, 0);
        wprintw(fenetreEnTete,"//////////////////////////////////////////////////////");
        wprintw(fenetreEnTete, "/              Projet IN204 - Ray Tracing       /");
        wprintw(fenetreEnTete, "/                                                    /");
        wprintw(fenetreEnTete, "/             Auteurs: DOGGAZ Mariem          /");
        wprintw(fenetreEnTete, "/                  AZZOUZ Salima           /");
        wprintw(fenetreEnTete, "/                                                    /");
        wprintw(fenetreEnTete, "//////////////////////////////////////////////////////");
        wrefresh(fenetreEnTete);
    }

    void InterfaceTerminal::initialiserFenetreOpt() {
        werase(fenetreOpt);
        wmove(fenetreOpt, 0, 0);
        wprintw(fenetreOpt, "Appuyez sur la touche indiquée pour effectuer une action\n");
        wprintw(fenetreOpt, "Entrée - Rendre la scène dans une fenêtre\n");
        wprintw(fenetreOpt, "c - Créer une nouvelle scène\n");
        wprintw(fenetreOpt, "r - Récupérer la scène à partir d'un fichier XML\n");
        wprintw(fenetreOpt, "p - Charger une scène exemple\n");
        wprintw(fenetreOpt, "s - Sauvegarder la scène au format XML\n");
        wprintw(fenetreOpt, "i - Sauvegarder la scène au format image\n");
        wprintw(fenetreOpt, "q - quitter\n");
        wrefresh(fenetreOpt);

        sf::FloatRect zoneVisible;

        auto c = wgetch(fenetreSaisie);
        switch(c) {
            case '\n':
                moteurRT.commencerTravail();
                mvwprintw(fenetreOpt, 10, 0, "Travail en cours... attendez la fin du rendu avant d'appuyer sur une touche");
                wrefresh(fenetreOpt);
                break;
            case 'c':
                nouvelleScene();
                wrefresh(fenetreOpt);
                break;
            case 'r':
                recupererXML();
                break;
            case 'p':
                moteurRT =MoteurRendu();
                zoneVisible = sf::FloatRect(0, 0, moteurRT.obtenirLargeurImage(), moteurRT.obtenirHauteurImage());
                fenetreSFML.setView(sf::View(zoneVisible));
                wmove(fenetreOpt, 10, 0);
                wclrtoeol(fenetreOpt);
                wprintw(fenetreOpt, "Scène exemple chargée");
                mvwprintw(fenetreOpt, 11, 0, "Appuyez sur entrée pour revenir");
                wrefresh(fenetreOpt);
                c = wgetch(fenetreSaisie);
                while(c != '\n') {c = wgetch(fenetreSaisie); }
                wrefresh(fenetreOpt);
                break;
            case 's':
                enregistrerXML();
                break;
            case 'i':
                if (moteurRT.aImagePret()) {
                    enregistrerImage();
                    mvwprintw(fenetreOpt, 10, 0, "Image enregistrée");
                    wrefresh(fenetreOpt);
                }
                else {
                    mvwprintw(fenetreOpt, 10, 0, "Vous devez d'abord rendre une scène");
                    mvwprintw(fenetreOpt, 11, 0, "Appuyez sur entrée pour revenir");
                    wrefresh(fenetreOpt);
                    c = wgetch(fenetreSaisie);
                    while(c != '\n') {c = wgetch(fenetreSaisie); }
                }
                break;
            case 'q':
                tempsDeFermer = true;
                erase();
                break;
            default:
                if(has_colors == FALSE) {
                    start_color();
                    init_pair(1, COLOR_RED, COLOR_BLACK);
                    attron(COLOR_PAIR(1));
                }

                mvwprintw(fenetreOpt, 10, 0, "Option invalide!");
                wrefresh(fenetreOpt);
        }
    }

    std::string InterfaceTerminal::obtenirParametre(int ligne) {
        wmove(fenetreOpt, ligne, 0);
        wclrtoeol(fenetreOpt);
        wrefresh(fenetreOpt);
        keypad(fenetreSaisie, true);
        auto c = wgetch(fenetreSaisie);
        std::string valeur;
        while(c != '\n') {
            if (c == KEY_BACKSPACE || c == KEY_DC || c == 8) {
                wrefresh(fenetreOpt);
                wmove(fenetreOpt, ligne, 0);
                wrefresh(fenetreOpt);
                wclrtoeol(fenetreOpt);
                wrefresh(fenetreOpt);
                valeur.pop_back();
            }
            else {
                valeur.push_back(c);
            }
            mvwprintw(fenetreOpt, ligne, 0, valeur.c_str());
            wrefresh(fenetreOpt);
            c = wgetch(fenetreSaisie);
        }
        keypad(fenetreSaisie, false);
        return valeur;
    }

    std::string InterfaceTerminal::obtenirNomFichier() {
        werase(fenetreOpt);
        wmove(fenetreOpt, 0, 0);
        wprintw(fenetreOpt, "Chemin et nom du fichier :\n");
        wrefresh(fenetreOpt);

        return obtenirParametre(1);
    }

    double InterfaceTerminal::obtenirDoubleParametre(int ligne) {
        double val;
        while (true) {
            try {
                std::string s = obtenirParametre(ligne);
                val = std::stod(s);
                break;
            }
            catch (std::exception& e) {
                mvwprintw(fenetreOpt, ligne +1, 0, "Erreur dans la valeur. Réessayez");
                wrefresh(fenetreOpt);
            }
        }
        wmove(fenetreOpt, ligne+1, 0);
        wclrtoeol(fenetreOpt);
        wrefresh(fenetreOpt);

        return val;
    }

    int InterfaceTerminal::obtenirEntierParametre(int ligne) {
        int val;
        while (true) {
            try {
                std::string s = obtenirParametre(ligne);
                val = std::stoi(s);
                break;
            }
            catch (std::exception& e) {
                mvwprintw(fenetreOpt, ligne +1, 0, "Erreur dans la valeur. Réessayez");
                wrefresh(fenetreOpt);
            }
        }
        wmove(fenetreOpt, ligne+1, 0);
        wclrtoeol(fenetreOpt);
        wrefresh(fenetreOpt);

        return val;
    }

    point InterfaceTerminal::obtenirpointParametre(int ligne) {
         point val;
        char delimiteur = ',';
        while (true) {
            try {
                std::string s = obtenirParametre(ligne);
                int i = 0;
                for (char c : s) {
                    if (c == ',') i++;
                }

                if (i != 2) throw std::invalid_argument("Pas le bon nombre de virgules");

                std::string x_str = s.substr(0, s.find(delimiteur));

                s = s.substr(s.find(delimiteur)+1);
                std::string y_str = s.substr(0, s.find(delimiteur));

                std::string z_str = s.substr(s.find(delimiteur)+1);

                val[0] = std::stod(x_str);
                val[1] = std::stod(y_str);
                val[2] = std::stod(z_str);

                break;
            }
            catch (std::exception& e) {
                mvwprintw(fenetreOpt, ligne +1, 0, "Erreur de valeur. Réessayez");
                wrefresh(fenetreOpt);
            }
        }
        wmove(fenetreOpt, ligne+1, 0);
        wclrtoeol(fenetreOpt);
        wrefresh(fenetreOpt);
        return val;
    }


   void InterfaceTerminal::recupererXML() {
    std::string filename;

    while(true) {
        filename = obtenirNomFichier();
        try {
            moteurRT = MoteurRendu(filename.c_str());
            break;
        }
        catch(std::exception& e) {
            mvwprintw(fenetreOpt, 10, 0, "Error while handling file, try again or another file");
            mvwprintw(fenetreOpt, 11, 0, "Press enter to try again");
            wrefresh(fenetreOpt);
            auto c = wgetch(fenetreSaisie);
            while(c != '\n') {c = wgetch(fenetreSaisie); }
        }
    }

    // update the view to the new size of the window
    sf::FloatRect visibleArea(0, 0, moteurRT.obtenirLargeurImage(), moteurRT.obtenirHauteurImage());
    fenetreSFML.setView(sf::View(visibleArea));
    mvwprintw(fenetreOpt, 10, 0, "Loaded! Press enter to return");
    wrefresh(fenetreOpt);
    auto c = wgetch(fenetreSaisie);
    while(c != '\n') {c = wgetch(fenetreSaisie); }
}

void InterfaceTerminal::enregistrerXML() {
    auto filename = obtenirNomFichier();
    moteurRT.sauvegarderDocumentXml(filename.c_str());
    mvwprintw(fenetreOpt, 10, 0, "Saved! Press enter to return");
    wrefresh(fenetreOpt);
    auto c = wgetch(fenetreSaisie);
    while(c != '\n') {c = wgetch(fenetreSaisie); }
}

void InterfaceTerminal::enregistrerImage() {
    auto filename = obtenirNomFichier();
    moteurRT.obtenirTexture().copyToImage().saveToFile(filename);
    mvwprintw(fenetreOpt, 10, 0, "Saved! Press enter to return");
    wrefresh(fenetreOpt);
    auto c = wgetch(fenetreSaisie);
    while(c != '\n') {c = wgetch(fenetreSaisie); }
}

void InterfaceTerminal::nouvelleScene() {
    int line = 0;
    werase(fenetreOpt);
    wmove(fenetreOpt, 0, 0);

    int imgHeight, imgWidth, samples_per_pixel, max_depth;

    mvwprintw(fenetreOpt, line++, 0, "------- Image Parameters -------");

    mvwprintw(fenetreOpt, line++, 0, "Image Width: ");
    imgWidth = obtenirEntierParametre(line++);

    mvwprintw(fenetreOpt, line++, 0, "Image height: ");
    imgHeight = obtenirEntierParametre(line++);

    mvwprintw(fenetreOpt, line++, 0, "Samples Per Pixel: ");
    samples_per_pixel = obtenirEntierParametre(line++);

    mvwprintw(fenetreOpt, line++, 0, "Max Depth: ");
    max_depth = obtenirEntierParametre(line++);

    moteurRT = MoteurRendu(imgWidth, imgHeight, samples_per_pixel, max_depth);

    line = 0;
    werase(fenetreOpt);
    wmove(fenetreOpt, 0, 0);
    point lookfrom, lookat, vup;
    double vfov, aperture, focus_dist, time0, time1;

    mvwprintw(fenetreOpt, line++, 0, "------- Camera Parameters -------");

    mvwprintw(fenetreOpt, line++, 0, "Camera Origin point: (ex: \"13, 2, 3\")");
    lookfrom = obtenirpointParametre(line++);

    mvwprintw(fenetreOpt, line++, 0, "point the camera is looking at: (ex: \"0, 0, 0\")");
    lookat = obtenirpointParametre(line++);

    mvwprintw(fenetreOpt, line++, 0, "View-up-vector vector: (ex: horizontal angle \"0, 1, 0\")");
    vup = lookfrom + obtenirpointParametre(line++);

    mvwprintw(fenetreOpt, line++, 0, "Vertical Field of View: ex: \"20.0\"");
    vfov = obtenirDoubleParametre(line++);

    mvwprintw(fenetreOpt, line++, 0, "Aperture: ex: \"0.1\"");
    aperture = obtenirDoubleParametre(line++);

    mvwprintw(fenetreOpt, line++, 0, "Focus Distance: ex: \"10.0\"");
    focus_dist = obtenirDoubleParametre(line++);

    mvwprintw(fenetreOpt, line++, 0, "Time0: ex: \"0.0\"");
    time0 = obtenirDoubleParametre(line++);

    mvwprintw(fenetreOpt, line++, 0, "Time1: \"0.0\"");
    time1 = obtenirDoubleParametre(line++);

    moteurRT.definircamera(lookfrom, lookat, vup, vfov, aperture, focus_dist, time0, time1);

    bool exit = false;
    while(!exit) {
        line = 0;
        werase(fenetreOpt);
        wmove(fenetreOpt, line, 0);

        mvwprintw(fenetreOpt, line++, 0, "------- World Items -------");
        mvwprintw(fenetreOpt, line++, 0, "Select an Item to add");
        mvwprintw(fenetreOpt, line++, 0, "1 - Sphere");
        mvwprintw(fenetreOpt, line++, 0, "2 - Moving Sphere");
        mvwprintw(fenetreOpt, line++, 0, "0 - Exit");
        mvwprintw(fenetreOpt, line++, 0, "OBS: Don't Forget the ground material, we use a big sphere");

        wrefresh(fenetreOpt);

        int choice = obtenirEntierParametre(line++);
        switch (choice) {
            case 1:
                moteurRT.ajouterAuMonde(creerSphere(line));
                break;
            case 2:
                moteurRT.ajouterAuMonde(creerSphereMobile(line));
                break;
            case 0:
                exit = true;
                break;
            default:
                mvwprintw(fenetreOpt, line+5, 0, "Invalid option!");
                wrefresh(fenetreOpt);
        }
    }

    sf::FloatRect visibleArea(0, 0, moteurRT.obtenirHauteurImage(), moteurRT.obtenirLargeurImage());
    fenetreSFML.setView(sf::View(visibleArea));
}

std::shared_ptr<Mobile_Sphere> InterfaceTerminal::creerSphereMobile(int line) {
    mvwprintw(fenetreOpt, line++, 0, "------- Moving Sphere Parameters -------");

    point center0, center1;
    double radius, time0, time1;
    mvwprintw(fenetreOpt, line++, 0, "Center of the sphere at time 0(ex: \"1, 2, 3\"): ");
    center0 = obtenirpointParametre(line++);
    mvwprintw(fenetreOpt, line++, 0, "Time 0: ");
    time0 = obtenirDoubleParametre(line++);

    mvwprintw(fenetreOpt, line++, 0, "Center of the sphere at time 1(ex: \"1, 2, 3\"): ");
    center1 = obtenirpointParametre(line++);
    mvwprintw(fenetreOpt, line++, 0, "Time 1: ");
    time1 = obtenirDoubleParametre(line++);

    mvwprintw(fenetreOpt, line++, 0, "Radius: ");
    radius = obtenirDoubleParametre(line++);

    return std::make_shared<Mobile_Sphere>(center0, center1, time0, time1, radius, selectionnerMateriau(line));
}

std::shared_ptr<SphereObject> InterfaceTerminal::creerSphere(int line) {
    mvwprintw(fenetreOpt, line++, 0, "------- Sphere Parameters -------");

    point center;
    double radius;
    mvwprintw(fenetreOpt, line++, 0, "Center of the sphere (ex: \"1, 2, 3\"): ");
    center = obtenirpointParametre(line++);
    mvwprintw(fenetreOpt, line++, 0, "Radius: ");
    radius = obtenirDoubleParametre(line++);

    return std::make_shared<SphereObject>(center, radius, selectionnerMateriau(line));
}

std::shared_ptr<materiau> InterfaceTerminal::selectionnerMateriau(int line) {
    mvwprintw(fenetreOpt, line++, 0, "------- Material Parameters -------");
    mvwprintw(fenetreOpt, line++, 0, "Select a material");
    mvwprintw(fenetreOpt, line++, 0, "1 - Lambertian");
    mvwprintw(fenetreOpt, line++, 0, "2 - Metal");
    mvwprintw(fenetreOpt, line++, 0, "3 - Dielectric");

    while(true) {
        int choice = obtenirEntierParametre(line++);
        point couleur;
        switch (choice) {
            case 1:
                mvwprintw(fenetreOpt, line++, 0, "couleur R, G, B (ex: \"0.5, 0.5, 0.5\"): ");
                couleur = obtenirpointParametre(line++);

                return std::make_shared<LambertianMateriau>(couleur);
            case 2:
                double fuzz;
                mvwprintw(fenetreOpt, line++, 0, "couleur R, G, B (ex: \"0.5, 0.5, 0.5\"): ");
                couleur = obtenirpointParametre(line++);
                mvwprintw(fenetreOpt, line++, 0, "Fuzz: ex: 2.0");
                fuzz = obtenirDoubleParametre(line++);

                return std::make_shared<MetalMateriau>(couleur, fuzz);

            case 3:
                double ir;
                mvwprintw(fenetreOpt, line++, 0, "Index of refraction: ex: 2.0");
                ir = obtenirDoubleParametre(line++);

                return std::make_shared<DielectricMateriau>(ir);
            default:
                mvwprintw(fenetreOpt, line+5, 0, "Invalid option!");
                wrefresh(fenetreOpt);
        }
    }
}


#endif
