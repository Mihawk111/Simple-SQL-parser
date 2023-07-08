#include "util.hpp"
using namespace std;

int main()
{
    fstream fOut("output.txt", ios::out | ios::trunc);


    cout << "--> Reading Productions ..." << endl;
    fstream fProd("input_prod.txt", ios::in);
    pair<vector<string>, Item> lol1 = readProductionsAsString(fProd);
    vector<string> prodStr = lol1.first;
    Item startSymbol = lol1.second;
    fProd.close();
    cout << "--> Reading Done ..." << endl << endl;

    cout << "--> Testing String Read ..." << endl;
    fOut << "RAW READ PRODUCTIONS (STRING) -->  [ Size = " << prodStr.size() << " ]" << endl;
    for(string st: prodStr) fOut << st << endl; fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Testing Done ..." << endl << endl;

    cout << "--> Parsing Productions ..." << endl;
    pair<Map, pair<Set, Set>> lol2 = parseProductionsfromInput(prodStr);
    Map prodMap = lol2.first;
    Set symbolSet = lol2.second.first;
    Set terminalSet = lol2.second.second;
    fOut << "READ PRODUCTIONS [ Start Symbol = " << startSymbol << " ] -->" << endl;
    printProductionList(prodMap, symbolSet, terminalSet, fOut); fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Parsing Done ..." << endl << endl;

    cout << "--> Removing left recursions ..." << endl;
    pair<Map, Set> lol3 = removeImmediateLeftRecursion(prodMap);
    prodMap = lol3.first;
    Set extraSymbols = lol3.second;
    for(Item symbol :extraSymbols) symbolSet.insert(symbol);
    Action::setProdMap(&prodMap);
    fOut << "LEFT RECURSION REMOVED PRODUCTIONS -->" << endl;
    printProductionList(prodMap, symbolSet, terminalSet, fOut); fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Left Recursions Removed ..." << endl << endl;

    cout << "--> Generating nullable map ..." << endl;
    MapNullable mapNullable = generateNullableMap(prodMap, symbolSet);
    fOut << "NULLABLE MAP -->" << endl;
    for(auto lol: mapNullable){ Item i = lol.first; fOut << i << " -> " << ((lol.second)? "True": "False") << endl; }
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Generation done ..." << endl << endl;

    cout << "--> Generating first map ..." << endl;
    MapFirst mapFirst = generateFirstFromProductions(prodMap, symbolSet, mapNullable);
    fOut << "FIRST MAP -->" << endl;
    for(auto lol: mapFirst){ Item i = lol.first; fOut << i << " -> { "; bool b = false; for(Item a: lol.second){ if(b) fOut << ", "; b = true; fOut << a << " "; } fOut << "}" << endl; }
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Generation done ..." << endl << endl;

    cout << "--> Generating follow map ..." << endl;
    MapFollow mapFollow = generateFollowFromProductions(prodMap, symbolSet, mapNullable, mapFirst, startSymbol);
    fOut << "FOLLOW MAP -->" << endl;
    for(auto lol: mapFollow){ Item i = lol.first; fOut << i << " -> { "; bool b = false; for(Item a: lol.second){ if(b) fOut << ", "; b = true; fOut << a << " "; } fOut << "}" << endl; }
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Generation done ..." << endl << endl;

    cout << "--> Generating LL(1) Parsing Table ..." << endl;
    ParsingTable parsingTable = generateLl1ParsingTable(prodMap, symbolSet, mapNullable, mapFirst, mapFollow);
    fOut << "LL(1) PARSING TABLE -->" << endl << endl;
    printParsingTable(parsingTable, symbolSet, terminalSet, fOut);
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Generation done ..." << endl << endl;

    cout << "--> Augmenting LL(1) Parsing Table with error recovery ..." << endl;
    augmentLl1ParsingTableWithErrorRecovery(prodMap, symbolSet, terminalSet, mapNullable, mapFirst, mapFollow, parsingTable);
    fOut << "AUGMENTED LL(1) PARSING TABLE -->" << endl << endl;
    printParsingTable(parsingTable, symbolSet, terminalSet, fOut);
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Augmentation done ..." << endl << endl;

    cout << "--> Reading Token Stream ..." << endl;
    fstream fInTokenStream("input_tokens.txt");
    vector<Token> tokenStream = readTokenStreamFromFile(fInTokenStream);
    fInTokenStream.close();
    fOut << "READ TOKEN STREAM -->" << endl;
    printReadTokenStream(tokenStream, fOut);
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Reading done ..." << endl << endl;

    cout << "--> Parsing using LL(1) Parsing Table ..." << endl;
    pair<pair<bool, string>, ParsingHistory> lol4 = parseTokenStreamUsingParsingTable(tokenStream, symbolSet, terminalSet, startSymbol, parsingTable, prodMap);
    bool errorFoundParsingResult = lol4.first.first;
    string parsingErrorInfo = lol4.first.second;
    ParsingHistory parsingHistory = lol4.second;
    fOut << "PARSING RESULT: " << ((!errorFoundParsingResult)? "TRUE": "FALSE") << endl;
    fOut << "PARSING ERROR INFO: " << parsingErrorInfo << endl << endl;
    printParsingHistory(parsingHistory, fOut);
    fOut << endl << "___________________________________________________________________________" << endl << endl;
    cout << "--> Parsing done ..." << endl << endl;


    fOut.close();

    cout << "DONE !!!" << endl;

    return 0;
}