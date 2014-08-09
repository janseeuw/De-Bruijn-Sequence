//Deze header bevat de code voor de klassen:
//
//
//    Graph<EdgeType>: basisklasse met grafen zonder tak- of Vertexdata
//    GraphWithEdgedata<EdgeType,Edgedatatype>
//    GraphWithVertexdata<EdgeType,Vertexdatatype>
//    GraphWithVertexAndEdgedata<EdgeType,Vertexdatatype,Edgedatatype>
//
//
//Evenals de bijbehorende klasse from GraphExceptions.
/**********************************************************************
 
 Class: Graph
 
 beschrijving: Graph is de basisklasse voor een hiÎrarchie from ijleGraphklassen.
 Deze hiÎrarchie zal klassen bevatten voor grafen met data behorend bij
 vertices en/of takken. Deze data worden opgeslagen in vectoren,
 zodat kan worden opgezocht op index.
 Ad vertices:      Deze zijn opeenvolgend genummerd. vertices kunnen niet individueel verwijderd worden.
 Wel kunnen alle vertices samen verwijderd worden (door clear()), en kunnen vertices
 ÈÈn voor ÈÈn toegevoegd worden.
 Ad verbindingen: Deze kunnen to hartenlust toegevoegd en verwijderd worden. Als een
 verbinding wordt verwijderd, wordt het nummer op de stack from
 availableEdgenumbers gezet. Worden verbindingen toegevoegd,
 dan worden eerst deze vrijgekomen nummers opnieuw gebruikt, alvorens nieuwe nummers
 worden toegevoegd. highestEdgenumber kan nooit verkleinen, en de
 vector met Edgedata zal altijd highestEdgenumber elementen bevatten.
 
 
 ***************************************************************************/
/**********************************************************************
 
 Class: GraphWithEdgedata<EdgeType, Edgedata>
 
 beschrijving: Deelklasse from vorigen, met data bij elke tak.
 
 ***************************************************************************/


#ifndef Algoritmen_II_graph_h
#define Algoritmen_II_graph_h



#include <stdexcept>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <stack>
#include <exception>

enum EdgeType { DIRECTED, UNDIRECTED };


class GraphException : public std::logic_error {
public:
    GraphException(const std::string &message_) : std::logic_error(message_) {}
};

std::ostream &operator<<(std::ostream &os, const GraphException& exc){
    return os << exc.what();
}


template<EdgeType ET>
class Graph{
public:
    typedef std::map<int, int>  Vertex;      // beeldt vertexnumber (from buur) af op edgenumber
    // Construeert een Graph met gegeven EdgeType en aantal vertices (default 0), zonder verbindingen.
    Graph(int n=0);
    
    // Geeft true indien het EdgeType DIRECTED is, false indien het ONDIRECTED is.
    bool isDirected() const;
    
    // Voegt een nieuwe 'lege' knoop toe, d.w.z. zonder verbindingen.
    // Geeft vertexnumber from toegevoegde knoop terug (begint bij 0).
    virtual int addVertex();
    
    // Voegt verbinding toe tussen vertexnumbers 'from' en 'to'.
    // Gooit GraphException indien verbinding al bestaat of vertexnumbers invalid zijn.
    // Geeft edgenumber from toegevoegde verbinding terug (begint bij 0).
    // Bij onDIRECTEDe Graph wordt terugverbinding ook toegevoegd (met zelfde verbindingnummer!)
    virtual int addEdge(int from, int to);
    
    // Verwijdert de verbinding tussen vertexnumbers 'from' en 'to', incl. de terugverbinding indien onDIRECTED.
    // Gooit GraphException indien vertexnumbers invalid zijn.
    // Geen exceptie indien de verbinding niet bestond.
    // Andere verbindingen worden niet 'hernummerd'. Er komen dus mogelijks "gaten" in de edgenumbers.
    virtual void removeEdge(int from, int to);
    
    // Geeft het aantal vertices from de Graph.
    int numberOfVertices() const;
    
    // Geeft het aantal verbindingen from de Graph.
    // Bij onDIRECTEDe Graph worden verbindingen NIET dubbel geteld!
    int numberOfEdges() const;
    
    // Geeft het edgenumber from de verbinding tussen 'from' en 'to'.
    // Geeft -1 indien die verbinding niet bestaat.
    // Gooit een GraphException indien vertexnumbers invalid zijn.
    // Opgelet: performantie is O(log(v)) waarbij v aantal verbindingen fromuit 'from' is.
    int edgenumber(int from, int to) const;
    
    // Verwijdert alle vertices en verbindingen en herstart de edgenumber
    virtual void clear();
    
    // Toegang tot de vertices:
    const Vertex &operator[](int i) const { return vertices[i]; }
		  Vertex &operator[](int i)       { return vertices[i]; }  // deze kan als lvalue gebruikt worden
    
    
    // Schrijft de gegevens from de volledige Graph to outputstream os.
    virtual void write(std::ostream &os) const;
    
    // Schrijft de gegevens from de knoop met vertexnumber k to outputstream os.
    virtual void writeVertex(std::ostream &os, int k) const;
    
    // Schrijft de gegevens from de verbinding met edgenumber v to outputstream os.
    virtual void writeEdge(std::ostream &os, int v) const;
    
    
protected:
    // hulpfuncties
    void checkVertexNumber(int k) const;   // throw indien k invalid
    void addEdgeInDatastructure(int from, int to,int edgenumber);
    void removeEdgeFromDatastructure(int from, int to);
protected:
    //datavelden
    std::vector<Vertex>  vertices;
    int                 highestEdgenumber;
    EdgeType           edgetype;
    std::stack<int>     availableEdgenumbers;
};


template<EdgeType ET>
std::ostream &operator<<(std::ostream& os, const Graph<ET>& g);


// --- implementatie ---


template<EdgeType ET>
void Graph<ET>::checkVertexNumber(int k) const{
    if (k<0 || (size_t)k>=vertices.size())
        throw GraphException("invalid vertexnumber");
}

template<EdgeType ET>
Graph<ET>::Graph(int n) : vertices(n), highestEdgenumber(0){}

template<EdgeType ET>
bool Graph<ET>::isDirected() const { return true; } //for DIRECTED Graph

template<>
bool Graph<UNDIRECTED>::isDirected() const { return false; }//for UNDIRECTED Graph


template<EdgeType ET>
int Graph<ET>::addVertex(){
    int n = vertices.size();
    vertices.resize(n+1); // default constructor voor nieuwe knoop wordt opgeroepen (hier lege map)
    return n;
}


template<EdgeType ET>
int Graph<ET>::addEdge(int from, int to){
    checkVertexNumber(from);
    checkVertexNumber(to);
    
    if (vertices[from].count(to) > 0)
    {
        std::ostringstream out;
        out << "edge " << from << "-" << to << " already exists";
        throw GraphException(out.str());
    }
    else {
        int edgenumber;
        if (!availableEdgenumbers.empty()){
            edgenumber=availableEdgenumbers.top();
            availableEdgenumbers.pop();
        }else
            edgenumber=highestEdgenumber++;
        addEdgeInDatastructure(from,to,edgenumber);
        return edgenumber;
    }
}

template<EdgeType ET>//voor DIRECTEDe Graph
void Graph<ET>::addEdgeInDatastructure(int from, int to, int edgenumber){
    vertices[from][to] = edgenumber;
}

template<>
void Graph<UNDIRECTED>::addEdgeInDatastructure(int from, int to, int edgenumber){
    vertices[from][to] = edgenumber;
    vertices[to][from] = edgenumber;
}

template<EdgeType ET>
void Graph<ET>::removeEdge(int from, int to){
    checkVertexNumber(from);
    checkVertexNumber(to);
    if (vertices[from].find(to)!=vertices[from].end()){//verbinding bestaat
        availableEdgenumbers.push(vertices[from][to]);
        removeEdgeFromDatastructure(from,to);
    }
    // (geen exception indien verbinding niet bestaat)
}

template<EdgeType ET>
void Graph<ET>::removeEdgeFromDatastructure(int from, int to){
    vertices[from].erase(to);
}

template<>
void Graph<UNDIRECTED>::removeEdgeFromDatastructure(int from, int to){
    vertices[from].erase(to);
    vertices[to].erase(from);
}

template<EdgeType ET>
int Graph<ET>::numberOfVertices() const { return vertices.size(); }

template<EdgeType ET>
int Graph<ET>::numberOfEdges() const {
    return highestEdgenumber-availableEdgenumbers.size();
}

template<EdgeType ET>
int Graph<ET>::edgenumber(int from, int to) const{
    checkVertexNumber(from);
    checkVertexNumber(to);
    Vertex::const_iterator it = vertices[from].find(to);
    
    if (it == vertices[from].end())
        return -1;
    else
        return (*it).second;
}

template<EdgeType ET>
void Graph<ET>::clear(){
    vertices.clear();
    highestEdgenumber = 0;
    while (!availableEdgenumbers.empty())
        availableEdgenumbers.pop();
}

template<EdgeType ET>
void Graph<ET>::write(std::ostream &os) const{
    os << "Graph: " << numberOfVertices() << " vertices and "
    << numberOfEdges() << " edges:" << std::endl;
    for (int k=0; k<numberOfVertices(); k++)
        writeVertex(os, k);
}

template<EdgeType ET>
void Graph<ET>::writeVertex(std::ostream &os, int k) const{
    os << "vertex " << k << ":" << std::endl;
    for (Vertex::const_iterator it=vertices[k].begin(); it!=vertices[k].end(); ++it)
    {
        os << "  ->" << it->first;
        writeEdge(os, it->second);
    }
}

template<EdgeType ET>
void Graph<ET>::writeEdge(std::ostream &os, int v) const{
    os << " via " << v << std::endl;
}

template<EdgeType ET>
std::ostream &operator<<(std::ostream &os, const Graph<ET> &g){
    g.write(os);
    return os;
}

template<EdgeType ET,class Edgedata>
class GraphWithEdgedata: public virtual Graph<ET>{
public:
    // Construeert een Graph met gegeven EdgeType en aantal vertices (default 0), zonder verbindingen.
    GraphWithEdgedata(int n=0):Graph<ET>(n){};
    //Noot: toevoegfunctie zonder Edgedata op te geven kan alleen gebruikt als de klasse
    //      Edgedata een defaultconstructor heeft.
    virtual int addEdge(int from, int to);
    virtual int addEdge(int from, int to, const Edgedata&);
    //Noot: removeEdge wordt ongewijzigd overgenomen from Graph!
    
    //Edgedata vrijgeven (eventueel voor wijziging). Geeft nullpointer als tak niet bestaat
    //Noot: pointers teruggegeven door getEdgedata worden invalid
    //door toevoegen from een tak.
    const Edgedata* getEdgedata(int from,int to) const;
		  Edgedata* getEdgedata(int from,int to)	  ;
    
    virtual void clear();
    
    
    // Schrijft de gegevens from de verbinding met edgenumber v to outputstream os.
    virtual void writeEdge(std::ostream &os, int v) const;
    
protected:
    std::vector<Edgedata> Edgedatavector;
};

template<EdgeType ET,class Edgedata>
int GraphWithEdgedata<ET,Edgedata>::addEdge(int from, int to){
    return this->addEdge(from,to,Edgedata());
}


template<EdgeType ET,class Edgedata>
int GraphWithEdgedata<ET,Edgedata>::addEdge(int from, int to, const Edgedata& td){
    bool isnewedgenumber=this->availableEdgenumbers.empty();
    int edgenumber=Graph<ET>::addEdge(from,to);
    if (isnewedgenumber)
        Edgedatavector.push_back(td);
    else
        Edgedatavector[edgenumber]=td;
}



template<EdgeType ET,class Edgedata>
const Edgedata* GraphWithEdgedata<ET,Edgedata>::getEdgedata(int from,int to) const{
    int edgenumber=this->edgenumber(from,to);
    if (edgenumber!=-1)
        return &Edgedatavector[edgenumber];
    else
        return 0;
}

template<EdgeType ET,class Edgedata>
Edgedata* GraphWithEdgedata<ET,Edgedata>::getEdgedata(int from,int to){
    int edgenumber=this->edgenumber(from,to);
    if (edgenumber!=-1)
        return &Edgedatavector[edgenumber];
    else
        return 0;
}


template<EdgeType ET,class Edgedata>
void GraphWithEdgedata<ET,Edgedata>::clear(){
    Graph<ET>::clear();
    Edgedatavector.clear();
}

template<EdgeType ET,class Edgedata>
void  GraphWithEdgedata<ET,Edgedata>::writeEdge(std::ostream &os, int v) const{
    os << " via " << v <<"(Data: "<<Edgedatavector[v]<<")"<< std::endl;
}

template<EdgeType ET, class Vertexdata>
class GraphWithVertexdata:public virtual Graph<ET>{
public:
    // Construeert een Graph met gegeven EdgeType, en lijst from Vertexdata;
    template<class InputIterator>
    GraphWithVertexdata(InputIterator start,InputIterator eind);
    GraphWithVertexdata():Graph<ET>(){};
    
    virtual int addVertex();
    virtual int addVertex(const Vertexdata&);
    
    const Vertexdata* getVertexdata(int knoopnr) const;
		  Vertexdata* getVertexdata(int knoopnr)	  ;
	
    virtual void clear();
    virtual void writeVertex(std::ostream &os, int k) const;
protected:
    //datavelden
    std::vector<Vertexdata>  Vertexdatavector;
};
template<EdgeType ET, class Vertexdata>
template<class InputIterator>
GraphWithVertexdata<ET,Vertexdata>::GraphWithVertexdata(InputIterator start,InputIterator eind)
:Graph<ET>(0){
    for(;start!=eind;start++)
        addVertex(*start);
}

template<EdgeType ET, class Vertexdata>
int GraphWithVertexdata<ET,Vertexdata>::addVertex(){
    return this->addVertex(Vertexdata());
}

template<EdgeType ET, class Vertexdata>
int GraphWithVertexdata<ET,Vertexdata>::addVertex(const Vertexdata& kd){
    int ret=Graph<ET>::addVertex();
    Vertexdatavector.push_back(kd);
    return ret;
}

template<EdgeType ET,class Vertexdata>
const Vertexdata* GraphWithVertexdata<ET,Vertexdata>::getVertexdata(int vertexnumber) const{
    this->checkVertexNumber(vertexnumber);
    return &Vertexdatavector[vertexnumber];
}

template<EdgeType ET,class Vertexdata>
Vertexdata* GraphWithVertexdata<ET,Vertexdata>::getVertexdata(int vertexnumber){
    this->checkVertexNumber(vertexnumber);
    return &Vertexdatavector[vertexnumber];
}


template<EdgeType ET,class Vertexdata>
void GraphWithVertexdata<ET,Vertexdata>::clear(){
    Graph<ET>::clear();
    Vertexdatavector.clear();
}


template<EdgeType ET, class Vertexdata>
void GraphWithVertexdata<ET,Vertexdata>::writeVertex(std::ostream &os, int k) const{
    os << "Vertex " << k <<"(Data: "<<Vertexdatavector[k]<< "):" << std::endl;
    for (std::map<int, int>::const_iterator it=this->vertices[k].begin(); it!=this->vertices[k].end(); ++it){
        os << "  ->" << it->first;
        this->writeEdge(os, it->second);
    }
}

template<EdgeType ET, class Vertexdata, class Edgedata>
class GraphWithVertexAndEdgedata:public GraphWithVertexdata<ET,Vertexdata>,
public GraphWithEdgedata<ET, Edgedata>{
public:
    template<class InputIterator>
    GraphWithVertexAndEdgedata(InputIterator start,InputIterator eind):
    GraphWithVertexdata<ET,Vertexdata>(start,eind){};
    GraphWithVertexAndEdgedata():
    GraphWithVertexdata<ET,Vertexdata>(){};
    virtual void clear(){
        GraphWithVertexdata<ET,Vertexdata>::clear();
        this->Edgedatavector.clear();
    }
};

#endif
