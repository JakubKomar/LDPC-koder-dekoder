
/**
 * @file matOp.cpp
 * @brief implementace specifických maticových operací.
 *
 * Tento soubor poskytuje specifické maticové operace pro potřeby LDPC kódování a dekódování.
 *
 * @author Bc. Jakub Komárek
 * @year 2023
 */
#include "matOp.hpp"

namespace matOp{

/**
 * Provede algoritmus Gaussovy-Jordanovy eliminace na matici H.
 *
 * Tato metoda přijímá matici H a provádí Gaussovu-Jordanovu eliminaci na této matici.
 * Volitelný parametr change určuje, zda chcete zachovat transformační matici P (defaultně false).
 * Výsledkem je pár matice A po provedení eliminace a volitelné transformační matice P.
 *
 * @param H Vstupní matice pro Gaussovu-Jordanovu eliminaci.
 * @param change Určuje, zda se má zachovat transformační matice P (defaultně false).
 * @return std::pair<cv::Mat, std::optional<cv::Mat>> Pár, kde první složka obsahuje upravenou matici A 
 * a druhá složka obsahuje volitelnou transformační matici P.
 */
std::pair<cv::Mat, std::optional<cv::Mat>> gaussJordan(const cv::Mat &H, bool change = false) {
    //init
    std::optional<cv::Mat> P_return = std::nullopt;
    cv::Mat A = H.clone(); 

    int m = A.rows;
    int n = A.cols;

    // Inicializace transformační matice P, pokud je vyžadováno
    cv::Mat P; 
    if (change) 
        P = cv::Mat::eye(m, m, CV_32S);

    int pivotOld = -1; 

    // Iterace přes sloupce matice
    for (int j = 0; j < n; ++j) { 
        // Filtrace hodnot pod diagonálou
        cv::Mat filterDown = A.rowRange(pivotOld + 1, m).col(j).t(); 

        // Nalezení lokace maximální hodnoty ve filtrované části - bere se první výskyt
        cv::Point maxLoc;
        cv::minMaxLoc(filterDown, NULL, NULL, NULL, &maxLoc);

        int pivot = maxLoc.x + pivotOld + 1;

        if (A.at<int>(pivot, j) != 0) {
            pivotOld += 1;

            // Prohození řádků, pokud pivot není na diagonále
            if (pivotOld != pivot) {
                cv::Mat aux = A.row(pivot).clone();
                A.row(pivotOld).copyTo(A.row(pivot));
                aux.copyTo(A.row(pivotOld));

                // Prohození řádků transformační matice P, pokud je vyžadováno
                if (change) {
                    auto aux = P.row(pivot).clone();
                    P.row(pivotOld).copyTo(P.row(pivot));
                    aux.copyTo(P.row(pivotOld));
                }
            }

            // Redukce hodnot pod diagonálou
            for (int i = 0; i < m; ++i) {
                if (i != pivotOld && A.at<int>(i, j) != 0) {
                    if (change) {
                        P.row(i) = cv::abs(P.row(i) - P.row(pivotOld));
                    }
                    A.row(i) = cv::abs(A.row(i) - A.row(pivotOld));
                }
            }
        }

        // Přerušení iterace, pokud jsou zpracovány všechny řádky
        if (pivotOld == m - 1) {
            break;
        }
    }

    if (change) 
        P_return = P;

    return std::make_pair(A, P_return);
}

/**
 * Provede binární násobení dvou matic s prvky typu integer.
 *
 * Tato metoda přijímá dvě matice X a Y s prvky typu integer, převede je na typ float,
 * provede násobení matic a vrátí výsledek jako matici s prvky modulo 2.
 *
 * @param X První matice pro binární násobení (s prvky typu integer).
 * @param Y Druhá matice pro binární násobení (s prvky typu integer).
 * @return cv::Mat Matice obsahující výsledek binárního násobení matic X a Y s prvky modulo 2.
 */
cv::Mat binaryProduct(cv::Mat X,  cv::Mat Y) {
    X.convertTo(X, CV_32F); // protože X*Y; bere pouze floaty z nějakého důvodu
    Y.convertTo(Y, CV_32F);

    cv::Mat A=X*Y;
    A.convertTo(A, CV_32S);

    A.forEach<int>([](int& item, const int* position) -> void {item %= 2; });

    return A;
}

/**
 * Násobení dvou matic s celočíselnými prvky.
 *
 * Tato metoda přijímá dvě matice X a Y s celočíselnými prvky, převede je na typ float,
 * provede násobení matic a vrátí výsledek jako matici s celočíselnými prvky.
 * Převod je udělán protože operace X*Y; bere pouze floaty z nějakého důvodu...
 * 
 * @param X První matice pro násobení (s celočíselnými prvky).
 * @param Y Druhá matice pro násobení (s celočíselnými prvky).
 * @return cv::Mat Matice obsahující výsledek násobení matic X a Y s celočíselnými prvky.
 */
cv::Mat matrixMulInt(cv::Mat X, cv::Mat Y) {

    X.convertTo(X, CV_32F); 
    Y.convertTo(Y, CV_32F);
    cv::Mat A=X*Y; // protože X*Y; bere pouze floaty z nějakého důvodu
    A.convertTo(A, CV_32S);

    return A;
}

/**
 * Speciální násobení dvou matic. Provede binární násobení dvou matic a vrátí výsledek jako jednobitový integer. 
 *
 * Tato metoda přijímá dvě matice a a b, provádí jejich násobení bez omezení na velikost
 * a vrací výsledek jako jednobitový integer (modulo 2). Předpokládá se, že výsledek má vyjít jako jedno číslo.
 * Funkce je užita pouze v metodě get_message.
 *
 * @param a První matice pro binární násobení.
 * @param b Druhá matice pro binární násobení (transponovaná).
 * @return int Výsledek binárního násobení modulo 2.
 */
int binaryProductInt(const cv::Mat& a, const cv::Mat& b) {   
    auto result = matOp::matrixMulNoRestrictions(a, b.t());
    
    return result.at<int>(0, 0) %2;
}

/**
 * Násobení dvou matic bez omezení na velikost - připomíná částečnou implementaci v numpy.
 *
 * Tato metoda přijímá dvě matice X a Y a provádí násobení matic bez omezení na velikost (počet řádků a sloupců).
 * Výsledkem je matice obsahující výsledky násobení prvků z matic X a Y.
 *
 * @param X První matice pro násobení.
 * @param Y Druhá matice pro násobení.
 * @return cv::Mat Matice obsahující výsledky násobení prvků z matic X a Y.
 */
cv::Mat matrixMulNoRestrictions(const cv::Mat &X, const cv::Mat &Y){

    cv::Mat result(1,X.rows, CV_32S, cv::Scalar(0));

    for (auto i = 0; i < X.rows; ++i) {
        for (auto j = 0; j < X.cols; ++j) {
            result.at<int>(0, i) += X.at<int>(i, j) * Y.at<int>(j, 0);
        }
    }

    return result;
}

/**
 * Provede Gaussovu eliminaci pro soustavu lineárních rovnic Ax = b.
 *
 * Tato metoda přijímá matici A (koeficienty rovnic) a vektor b (hodnoty na pravé straně rovnic)
 * a provede Gaussovu eliminaci pro řešení soustavy lineárních rovnic Ax = b.
 *
 * @param A Matice koeficientů rovnic.
 * @param b Vektor hodnot na pravé straně rovnic.
 * @return std::pair<cv::Mat, cv::Mat> Pár, kde první složka obsahuje upravenou matici A a druhá složka obsahuje upravený vektor b.
 */
std::pair<cv::Mat, cv::Mat> gaussElimination(cv::Mat A, cv::Mat b){
    
    int n = A.rows;
    int k = A.cols;

    for (int j = 0; j < std::min(k, n); ++j) {

         // Nalezení pivotu v daném sloupci
        std::vector<int> listDepivots;
        for (int i = j; i < n; ++i) {
            if (A.at<int>(i, j)) {
                listDepivots.push_back(i);
            }
        }

        int pivot;
        if (!listDepivots.empty()) 
            pivot = *std::min_element(listDepivots.begin(), listDepivots.end());      
        else
            continue;

        // Prohození řádků, aby pivot byl na diagonále
        if (pivot != j) {
            auto aux = A.row(j).clone();
            A.row(pivot).copyTo(A.row(j));
            aux.copyTo(A.row(pivot));

            int aux1 = b.at<int>(j);
            b.at<int>(j) = b.at<int>(pivot);
            b.at<int>(pivot) = aux1;
        }

        for (int i = j + 1; i < n; ++i) {
            if (A.at<int>(i, j)) {
                A.row(i) = cv::abs(A.row(i) - A.row(j));
                b.at<int>(i) = std::abs(b.at<int>(i) - b.at<int>(j));
            }
        }
    }
    return std::make_pair(A, b);
}
}// end of namespace matOp