#include "paper.h"
#include "sectioneditordlg.h"
#include <QKeyEvent>

struct _size {
        double width;
        double heigth;
    };

_size StandarSizes [] = {
    {841 , 1189},{594 , 841},{420 , 594},{297 , 420},
    {210 , 297}, {148 , 210},{105 , 148},{74 , 105},
    {52 , 74},      {37 , 52},      {1000 , 1414},  {707 , 1000},
    {500 , 707},    {353 , 500},    {250 , 353},    {176 , 250},
    {125 , 176},    {88 , 125},     {62 , 88},      {33 , 62},
    {31 , 44},      {163 , 229},    {105 , 241},    {110 , 220},
    {190.5 , 254},  {210 , 330},    {431.8 , 279.4},{215.9 , 355.6},
    {215.9 , 279.4},{279.4 , 431.8 }
                                    };

Paper::Paper(QGraphicsItem *parent) :
    QGraphicsRectItem(parent)
{    
    _inserting = false;
    mySize = A4;
    m_orientacion = Retrato;
    CFondo = Qt::NoBrush;
}

QRectF Paper::margin()
{
    return QRectF(cmToPx(margenSuperior()),
                  cmToPx(margenSuperior()),
                  this->rect().width() - cmToPx(margenSuperior()) - cmToPx(margenSuperior())- 5,
                  this->rect().height()- cmToPx(margenSuperior()) - cmToPx(margenSuperior())- 5);
}

QRectF Paper::paper()
{
    return QRectF(0,
                  0,
                  this->rect().width() -  5,
                  this->rect().height()-  5);
}

Paper::_Sizes Paper::StandartSize()
{
    return mySize;
}

Paper::_Orientacion Paper::Orientacion() const {
    return m_orientacion;

}

void Paper::setOrientacion(Paper::_Orientacion arg) {
    if (m_orientacion != arg) {
        QRectF r = this->rect();
        this->setRect(0,0, r.height(),r.width());
        m_orientacion = arg;
        emit orientacionChanged(arg);
    }
}

void Paper::setMargen(double arg)
{
    setmargenDerecho(arg);
    setmargenIzquierdo(arg);
    setmargenSuperior(arg);
    setmargenInferior(arg);
}

void Paper::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRectF r = this->rect();
    painter->fillRect(5,5,r.width()-5,r.height()-5,Qt::black);
    painter->fillRect(0,0,r.width()-5,r.height()-5,Qt::white);
    painter->fillRect(margin(),CFondo);
    painter->drawRect(0,0,r.width()-5,r.height()-5);
    painter->setPen(Qt::DotLine);
    painter->drawRect(margin());
}

void Paper::Fondo(QBrush fondo)
{
    CFondo = fondo;
    CFondo.setColor(Qt::gray);
}

void Paper::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(_inserting)
    {
        if(margin().contains(event->pos()))
        {
            if(seccionPool.isEmpty())
            {
                QMessageBox::warning(qApp->activeWindow(),tr("Sin secciones seleccionadas"),tr("Inserta una seccion desde el menu o doble click en el area de trabajo."));
                emit itemInserted();
                return;
            }
            _insertingPoint = event->pos();
            QList<Section*>::Iterator it;
            Section * target;
            for(it = seccionPool.begin();it!= seccionPool.end();++it)
            {
                target = (*it);
                if( mapRectToItem(this,QRectF(target->pos(),target->rect().size())).contains( mapToItem(this,_insertingPoint) ))
                    break;
            }
            switch (_insertingType)
            {
            case Paper::Campo:
                insertCampo(target);
                break;
            case Paper::Imagen:
                insertImagen(target);
                break;
            case Paper::Linea:
                insertLinea(target);
                break;
            case Paper::Label:
                insertLabel(target);
                break;
            case RoundRectIt:
                insertRoundRect(target);
                break;
            case Paper::CodeBarIt:
                insertCodeBar(target);
                break;
            case Paper::CampoRelacional:
                insertCampoRelacional(target);
                break;
            case Paper::Acumulador:
                insertAcumulador(target);
                break;
            case Paper::Parametro:
                insertParametro(target);
                break;
            default:
                break;
            }
            emit itemInserted();
        }
    }
    else
    {
        QList<QGraphicsItem*> list = this->scene()->selectedItems();
        for(int i=0; i<list.size();i++)
            list.at(i)->setSelected(false);
    }
}

void Paper::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    SectionEditorDlg dlg(this,qApp->activeWindow());
    dlg.exec();
}

void Paper::Seccion()
{
    SectionEditorDlg dlg(this,qApp->activeWindow());
    dlg.exec();
}

qreal Paper::cmToPx(double cm)
{
    double inch = cm * 0.3937008;
    return inch * 96.0;
}

qreal Paper::pxTocm(int px)
{
    double inch = px/96.0;
    return inch/0.3937008;
}

void Paper::insertRoundRect(Section * sec)
{
    QString name = QString("rect_%1").arg(qrand());
    RoundedRect * rect = new RoundedRect(name,this);
    rect->setMargins(this->margin());
    rect->setSize(100,100);
    rect->setPos(_insertingPoint.x()-50,_insertingPoint.y()-50);
    connect(rect,SIGNAL(moved(Container*)),this,SLOT(itemMoved(Container*)));    
    sec->_items.append(rect);
    itemPool.append(rect);
}

void Paper::insertLabel(Section * sec)
{
    QString name = QString("txt_%1").arg(qrand());
    CustomLabel * rect = new CustomLabel(name,this);
    rect->setMargins(this->margin());
    rect->setSize(100,15);
    rect->setPos(_insertingPoint.x()-20,_insertingPoint.y()-7);
    rect->setText(tr("Prueba de texto: Tamaño, orientacion, tipo de letra, color, posicion, negrita, cursiva, subrayado, centrada, etc."));
    rect->setfontSize(10);
    connect(rect,SIGNAL(moved(Container*)),this,SLOT(itemMoved(Container*)));
    sec->_items.append(rect);
    itemPool.append(rect);
}

void Paper::insertLinea(Section * sec)
{
    QString name = QString("lin_%1").arg(qrand());
    ReportLine * line = new ReportLine (name,this);
    line->setMargins(this->margin());
    line->setSize(200,15);
    line->setPos(_insertingPoint.x()-100,_insertingPoint.y()-7);
    connect(line,SIGNAL(moved(Container*)),this,SLOT(itemMoved(Container*)));
    sec->_items.append(line);
    itemPool.append(line);
}

void Paper::insertImagen(Section * sec )
{
    QString name = QString("img_%1").arg(qrand());
    ReportImage * img = new ReportImage(name,this);
    img->setruta("/usr/share/RecoverDrake/logo.png");
    img->setMargins(this->margin());
    img->setPos(_insertingPoint.x()-50,_insertingPoint.y()-7);
    connect(img,SIGNAL(moved(Container*)),this,SLOT(itemMoved(Container*)));
    sec->_items.append(img);
    itemPool.append(img);
}

void Paper::insertCampo(Section * sec)
{
    QString name = QString("cam_%1").arg(qrand());
    ReportField * fld = new ReportField(name,this);
    fld->setMargins(this->margin());
    fld->setSize(100,20);
    fld->setPos(_insertingPoint.x()-50,_insertingPoint.y()-7);
    connect(fld,SIGNAL(moved(Container*)),this,SLOT(itemMoved(Container*)));
    sec->_items.append(fld);
    itemPool.append(fld);
}

void Paper::insertCodeBar(Section * sec)
{
    QString name = QString("cod_%1").arg(qrand());
    CodeBar* code = new CodeBar(name,this);
    code->setMargins(this->margin());
    code->setcode("*123456789*");
    code->setvisibleCode(true);
    code->setPos(_insertingPoint.x()-100,_insertingPoint.y()-7);
    connect(code,SIGNAL(moved(Container*)),this,SLOT(itemMoved(Container*)));
    sec->_items.append(code);
    itemPool.append(code);
}

void Paper::insertCampoRelacional(Section * sec)
{
    QString name = QString("rel_%1").arg(qrand());
    RelationalField * fld = new RelationalField(name,this)   ;
    fld->setMargins(this->margin());
    fld->setSize(150,20);
    fld->setPos(_insertingPoint.x()-50,_insertingPoint.y()-7);
    connect(fld,SIGNAL(moved(Container*)),this,SLOT(itemMoved(Container*)));
    sec->_items.append(fld);
    itemPool.append(fld );
}

void Paper::insertParametro(Section *sec)
{
    QString name = QString("para_%1").arg(qrand());
    reportParama * fld = new reportParama(name,this)   ;
    fld->setMargins(this->margin());
    fld->setSize(150,20);
    fld->setText(":fecha");
    fld->setPos(_insertingPoint.x()-50,_insertingPoint.y()-7);
    connect(fld,SIGNAL(moved(Container*)),this,SLOT(itemMoved(Container*)));
    sec->_items.append(fld);
    itemPool.append(fld );
}

void Paper::insertAcumulador(Section *sec)
{
    QString name = QString("acum_%1").arg(qrand());
    ReportAcumulator * fld = new ReportAcumulator(name,this)   ;
    fld->setMargins(this->margin());
    fld->setSize(150,20);
    fld->setPos(_insertingPoint.x()-50,_insertingPoint.y()-7);
    connect(fld,SIGNAL(moved(Container*)),this,SLOT(itemMoved(Container*)));
    sec->_items.append(fld);
    itemPool.append(fld );
}

double Paper::margenSuperior() const {
    return m_margenSuperior;
}

int Paper::SFondo() const
{
    if (CFondo.style() == Qt::NoBrush)
        return 0;
    else
        return 1;
}

void Paper::setSize(double w, double h)
{
    this->setRect(0,0,cmToPx(w)+5,cmToPx(h)+5);
    scene()->setSceneRect(-10,-10,rect().width()+200,rect().height()+15);
}

void Paper::_insertSection(Section *sec)
{
    if(seccionPool.isEmpty())
        seccionPool.append(sec);
    else
    {
        bool inserted = false;
        Section::SectionType type = sec->sectionType();
        for(int i=0;i<seccionPool.size();i++)
        {
            if(seccionPool.at(i)->sectionType() > type)
            {
                seccionPool.insert(i,sec);
                inserted = true;
                break;
            }
        }
        if(!inserted)
            seccionPool.append(sec);
    }
}

void Paper::reCalculateSeccion(Section * secSender)
{
    int position = margin().top();
    int final = 0;
    QPair<Section *,int> maxHeigth;
    QListIterator<Section*> it(seccionPool);
    while (it.hasNext())
    {
        Section* sec = it.next();
        qreal oldP = sec->pos().ry();
        qreal diff = position-oldP;
        sec->setvPos(position);
        position += sec->Height()+1;

        QList<Container *>::Iterator itemIt;
        for(itemIt = sec->_items.begin();itemIt != sec->_items.end();++itemIt)
            (*itemIt)->setPos((*itemIt)->pos().rx(),(*itemIt)->pos().ry()+diff);

        if(sec->Height() > maxHeigth.second)
        {
            maxHeigth.first = sec;
            maxHeigth.second = sec->Height();
        }
        if(!it.hasNext())
            final = position ;
    }
    if (final > margin().bottom())
    {
        int diff = final - margin().bottom();
        if(secSender->Height() > diff)
        {
            secSender->setOnMaxsize(true);
            secSender->setHeight(secSender->Height() - diff);
            int position = margin().top();
            QListIterator<Section*> it(seccionPool);
            while(it.hasNext())
            {
                Section* sec = it.next();
                sec->setvPos(position);
                position += sec->Height()+1;
            }
        }
        else
        {
            maxHeigth.first->setHeight(maxHeigth.second - diff);
            int position = margin().top();
            QListIterator<Section*> it(seccionPool);
            while(it.hasNext())
            {
                Section* sec = it.next();
                sec->setvPos(position);
                position += sec->Height()+1;
            }
        }
    }
}

void Paper::itemMoved(Container * cont)
{
    QPointF pos = cont->pos();
    QList<Section *>::Iterator it;
    Section * oldSection = 0;
    Section * newSection = 0;
    for(it = seccionPool.begin();it != seccionPool.end(); ++it)
    {
        Section * aux = *it;
        if(aux->_items.contains(cont))
            oldSection = aux;
        if(mapRectToItem(this,QRectF(aux->pos(),aux->rect().size())).contains(pos))
            newSection = aux;
    }
    if(oldSection == newSection)
        return;
    if(oldSection)
        oldSection->_items.removeOne(cont);
    if(newSection)
        newSection->_items.append(cont);
}

void Paper::setSize(_Sizes siz, double w, double h, _Orientacion o)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
    mySize = siz;
    if(siz != QPrinter::Custom)
    {
        if(o== Retrato)
            this->setSize(StandarSizes[siz].width/10 , StandarSizes[siz].heigth/10);
        else
            this->setSize(StandarSizes[siz].heigth/10 , StandarSizes[siz].width/10);
    }
}

bool Paper::addSection(QString nombre, Section::SectionType type)
{
    bool valid = true;
    QListIterator<Section*> it(seccionPool);
    while (it.hasNext())
    {
        if(it.next()->SectionName() == nombre)
        {
            valid = false;
            break;
        }
    }
    if(!valid)
        return false;

    if(type != Section::Detail && type!= Section::PageHeader && type!=Section::PageFooter)
    {
        Section * seccion = new Section(this);
        seccion->setType(type);
        seccion->setSectionName(nombre);
        seccion->setMargin(margin());
        if(seccionPool.isEmpty())
        {
            seccionPool.append(seccion);
            seccion->setvPos(margin().top());
        }
        else
        {
            switch (type) {
            case Section::ReportHeader:
                seccionPool.prepend(seccion);
                break;
            case Section::PageFooter:
            case Section::Detail:
                _insertSection(seccion);
                break;
            default:
                seccionPool.append(seccion);
                break;
            }
            reCalculateSeccion(seccion);
        }
        connect(seccion,SIGNAL(resized(Section*)),this,SLOT(reCalculateSeccion(Section*)));
    }
    else if(type==Section::Detail)
    {
        DetailSection * seccion = new DetailSection(this);
        seccion->setType(type);
        seccion->setSectionName(nombre);
        seccion->setMargin(margin());
        if(seccionPool.isEmpty())
        {
            seccionPool.append(seccion);
            seccion->setvPos(margin().top());
        }
        else
        {
            _insertSection(seccion);
            reCalculateSeccion(seccion);
        }
        connect(seccion,SIGNAL(resized(Section*)),this,SLOT(reCalculateSeccion(Section*)));
    }
    else if(type==Section::PageFooter)
    {
        PageHeaderSection * seccion = new PageHeaderSection(this);
        seccion->setType(type);
        seccion->setSectionName(nombre);
        seccion->setMargin(margin());
        if(seccionPool.isEmpty())
        {
            seccionPool.append(seccion);
            seccion->setvPos(margin().top());
        }
        else
        {
            _insertSection(seccion);
            reCalculateSeccion(seccion);
        }
        connect(seccion,SIGNAL(resized(Section*)),this,SLOT(reCalculateSeccion(Section*)));
    }
    else
    {
        PageHeaderSection * seccion = new PageHeaderSection(this);
        seccion->setType(type);
        seccion->setSectionName(nombre);
        seccion->setMargin(margin());
        if(seccionPool.isEmpty())
        {
            seccionPool.append(seccion);
            seccion->setvPos(margin().top());
        }
        else
        {
            _insertSection(seccion);
            reCalculateSeccion(seccion);
        }
        connect(seccion,SIGNAL(resized(Section*)),this,SLOT(reCalculateSeccion(Section*)));
    }
    return true;
}

void Paper::removeSection(QString nombre)
{
    QListIterator<Section*> it(seccionPool);
    while (it.hasNext())
    {
        Section* sec = it.next();
        if(sec->SectionName() == nombre)
        {
            seccionPool.removeOne(sec);
            sec->deleteLater();
        }
    }
    reCalculateSeccion();
}

void Paper::prepareItemInsert(itemType type)
{
    _insertingType = type;
    _inserting = true;
}

void Paper::stopInsertingItems()
{
    _inserting = false;
}

bool Paper::parseXML(QString xml, QString & error)
{
    QListIterator<Section*> secIt(seccionPool);
    QListIterator<Container*> itemIt(itemPool);
    while(secIt.hasNext())
        secIt.next()->deleteLater();
    while(itemIt.hasNext())
        itemIt.next()->deleteLater();
    seccionPool.clear();
    itemPool.clear();
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(xml, false, &errorStr, &errorLine, &errorColumn))
    {
        error = QString(tr("Error: Error de analisis en linea %1, columna %2 : %3")).arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }
    else
    {
        QDomElement root = doc.documentElement();
        if (root.tagName() != "RDReports")
        {
            error = tr("Error: No es un archivo RDReports valido");
            return false;
        }
        else
        {
            QDomNode child = root.firstChild();
            while (!child.isNull())
            {
                QDomNode sections = child.firstChild();
                while (!sections.isNull())
                {
                    QDomElement secEle = sections.toElement();
                    if(secEle.tagName() == "Size")
                        this->setSize(secEle.attribute("w").toDouble(),secEle.attribute("h").toDouble());
                    if(secEle.tagName() == "Size")
                        this->m_orientacion = secEle.attribute("type") == "V" ? Retrato : Apaisado;
                    else if(secEle.tagName() == "Margin")
                    {
                        this->setmargenDerecho(secEle.attribute("rigth").toDouble());
                        this->setmargenIzquierdo(secEle.attribute("left").toDouble());
                        this->setmargenSuperior(secEle.attribute("top").toDouble());
                        this->setmargenInferior(secEle.attribute("bottom").toDouble());
                    }
                    else if(secEle.tagName() == "Section")
                    {
                        int typeOfSection = secEle.attribute("id").toDouble();
                        Section::SectionType type = static_cast<Section::SectionType>(typeOfSection);
                        if(type != Section::Detail && type != Section::PageHeader && type != Section::PageFooter)
                        {
                            Section * sec = new Section(this);
                            sec->setHeight(secEle.attribute("size").toDouble());
                            sec->setSectionName(secEle.attribute("name"));
                            sec->setMargin(margin());
                            sec->setType(type);
                            _insertSection(sec);
                            reCalculateSeccion(sec);
                            sec->parseXml(secEle.firstChild().toElement(), itemPool);
                            connect(sec,SIGNAL(resized(Section*)),this,SLOT(reCalculateSeccion(Section*)));
                        }
                        else if(type == Section::Detail)
                        {
                            DetailSection* sec = new DetailSection(this);
                            sec->setHeight(secEle.attribute("size").toDouble());
                            sec->setSectionName(secEle.attribute("name"));
                            sec->setType(type);
                            sec->setMargin(margin());                            
                            sec->setSqlGlobal(secEle.attribute("SqlGlobal"));
                            sec->setSqlInterno(secEle.attribute("SqlInterno"));
                            sec->setClausulaInterna(secEle.attribute("ClausulaInterna"));
                            sec->setcolorear(secEle.attribute("colored").toDouble());
                            sec->setcolor1(sec->ColorFromString(secEle.attribute("color1")));
                            sec->setuse2Colors(secEle.attribute("alternative").toDouble());
                            sec->setcolor2(sec->ColorFromString(secEle.attribute("color2")));
                            _insertSection(sec);
                            reCalculateSeccion(sec);
                            connect(sec,SIGNAL(resized(Section*)),this,SLOT(reCalculateSeccion(Section*)));
                            sec->parseXml(secEle.firstChild().toElement(), itemPool);                            
                        }
                        else
                        {
                            PageHeaderSection * sec = new PageHeaderSection(this);
                            sec->setOnFisrtPage(secEle.attribute("OnFistPage").toDouble());
                            sec->setHeight(secEle.attribute("size").toDouble());
                            sec->setSectionName(secEle.attribute("name"));
                            sec->setMargin(margin());
                            sec->setType(type);
                            _insertSection(sec);
                            reCalculateSeccion(sec);
                            sec->parseXml(secEle.firstChild().toElement(), itemPool);
                            connect(sec,SIGNAL(resized(Section*)),this,SLOT(reCalculateSeccion(Section*)));
                        }
                    }
                    sections = sections.nextSibling();
                }
                child = child.nextSibling();
            }
        }
    }
    for(int i= 0;i < itemPool.size();i++)
        connect(itemPool.at(i),SIGNAL(moved(Container*)),this,SLOT(itemMoved(Container*)));
    return true;
}

int Paper::save(QString file)
{
    QFile f(file);
    if(f.open(QFile::WriteOnly))
    {
        QTextStream out(&f);
        const int Indent = 4;
        QDomDocument doc;
        QDomElement root = doc.createElement("RDReports");
        QDomElement paperNode = doc.createElement("Paper");
        QDomElement siz = doc.createElement("Size");
        siz.setAttribute("w", QString::number( pxTocm(this->paper().width ()), 'f', 2));
        siz.setAttribute("h", QString::number( pxTocm(this->paper().height()), 'f', 2));
        QDomElement din = doc.createElement("StandartSize");
        din.setAttribute("type",this->mySize);
        QDomElement orientacion = doc.createElement("Orientation");
        orientacion.setAttribute("type",this->m_orientacion == Retrato ? "V" : "H");
        QDomElement margin = doc.createElement("Margin");
        margin.setAttribute("top",QString::number(this->margenSuperior(), 'f', 2));
        margin.setAttribute("bottom",QString::number(this->margenInferior(), 'f', 2));
        margin.setAttribute("left",QString::number(this->margenIzquierdo(), 'f', 2));
        margin.setAttribute("rigth",QString::number(this->margenDerecho(), 'f', 2));
        doc.appendChild(root);
        root.appendChild(paperNode);
        paperNode.appendChild(siz);
        paperNode.appendChild(din);
        paperNode.appendChild(orientacion);
        paperNode.appendChild(margin);
        QList<Section*>::Iterator it;
        QList<Container*> usedItems;
        QMap<QString,bool> querys;
        for(it=seccionPool.begin();it!=seccionPool.end();++it)
            paperNode.appendChild((*it)->xml(doc,usedItems,querys,seccionPool));
        QMapIterator<QString,bool> mIt(querys);
        while(mIt.hasNext())
        {
            QString s = mIt.next().key();
            if(!s.isEmpty())
            {
                QDomElement sql = doc.createElement("SQL");
                sql.setAttribute("target",s);
                paperNode.appendChild(sql);
            }
        }
        QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"ISO-8859-1\"");
        doc.insertBefore(xmlNode, doc.firstChild());
        doc.save(out, Indent);
        f.close();
        if (usedItems.size() < itemPool.size())
            return -2;
        else
            return 1;
    }
    else
        return -1;
}

QDomDocument Paper::preview()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("RDReports");
    QDomElement paperNode = doc.createElement("Paper");
    QDomElement siz = doc.createElement("Size");
    siz.setAttribute("w", QString::number( pxTocm(this->paper().width ()), 'f', 2));
    siz.setAttribute("h", QString::number( pxTocm(this->paper().height()), 'f', 2));
    QDomElement din = doc.createElement("StandartSize");
    din.setAttribute("type",this->mySize);
    QDomElement orientacion = doc.createElement("Orientation");
    orientacion.setAttribute("type",this->m_orientacion == Retrato ? "V" : "H");
    QDomElement margin = doc.createElement("Margin");
    margin.setAttribute("top",QString::number(this->margenSuperior(), 'f', 2));
    margin.setAttribute("bottom",QString::number(this->margenInferior(), 'f', 2));
    margin.setAttribute("left",QString::number(this->margenIzquierdo(), 'f', 2));
    margin.setAttribute("rigth",QString::number(this->margenDerecho(), 'f', 2));
    doc.appendChild(root);
    root.appendChild(paperNode);
    paperNode.appendChild(siz);
    paperNode.appendChild(din);
    paperNode.appendChild(orientacion);
    paperNode.appendChild(margin);
    QList<Section*>::Iterator it;
    QList<Container*> usedItems;
    QMap<QString,bool> querys;
    for(it=seccionPool.begin();it!=seccionPool.end();++it)
        paperNode.appendChild((*it)->xml(doc,usedItems,querys,seccionPool));
    QMapIterator<QString,bool> mIt(querys);
    while(mIt.hasNext())
    {
        QString s = mIt.next().key();
        if(!s.isEmpty())
        {
            QDomElement sql = doc.createElement("SQL");
            sql.setAttribute("target",s);
            paperNode.appendChild(sql);
        }
    }
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"ISO-8859-1\"");
    doc.insertBefore(xmlNode, doc.firstChild());
    QFile f("/usr/share/RecoverDrake/pre.xml");
    if(f.open(QFile::WriteOnly))
    {
        QTextStream t(&f);
        doc.save(t,4);
    }
    f.close();
    return doc;
}

void Paper::setmargenSuperior(double arg) {
    if (m_margenSuperior != arg) {
        m_margenSuperior = arg;
        emit margenSuperiorChanged(arg);
    }
}

double Paper::margenInferior() const {
    return m_margenInferiro;
}

double Paper::margenIzquierdo() const {
    return m_margenIzquierdo;
}

double Paper::margenDerecho() const {
    return m_margenDerecho;
}

void Paper::setmargenInferior(double arg) {
    if (m_margenInferiro != arg) {
        m_margenInferiro = arg;
        emit margenInferiroChanged(arg);
    }
}

void Paper::setmargenIzquierdo(double arg) {
    if (m_margenIzquierdo != arg) {
        m_margenIzquierdo = arg;
        emit margenIzquierdoChanged(arg);
    }
}

void Paper::setmargenDerecho(double arg) {
    if (m_margenDerecho != arg) {
        m_margenDerecho = arg;
        emit margenDerechoChanged(arg);
    }
}

QList<Section *> Paper::getSeccionPool() const
{
    return seccionPool;
}

void Paper::removeItems(QList<QGraphicsItem *> items)
{
    QListIterator<Container*> it(itemPool);    
    while(it.hasNext())
    {
        Container * cont = it.next();
        QList<Section *>::Iterator it;
        for(it = seccionPool.begin();it != seccionPool.end(); ++it)
        {
            Section * aux = *it;
            if(aux->_items.contains(cont))
            {
                aux->_items.removeOne(cont);
                break;
            }
        }
        if(items.contains(cont))
        {
            itemPool.removeOne(cont);
            cont->deleteLater();
        }
    }
}

void Paper::insertSection(Section *sec)
{
    _insertSection(sec);
}

void Paper::subirSeccion(QString name)
{
    int i = 0;
    for (;i< seccionPool.size();i++)
    {
        if(seccionPool.at(i)->SectionName() == name)
        {
            if(i!= 0)
            {
                seccionPool.swap(i,i-1);
                break;
            }
        }
    }
    reCalculateSeccion(seccionPool.at(i-1));
}

void Paper::bajarSeccion(QString name)
{
    int i = 0;
    for (;i< seccionPool.size();i++)
    {
        if(seccionPool.at(i)->SectionName() == name)
        {
            if(i!= seccionPool.size()-1)
            {
                seccionPool.swap(i,i+1);
                break;
            }
        }
    }
    reCalculateSeccion(seccionPool.at(i+1));
}

void Paper::borrarSeccion(QString name)
{
    int i = 0;
    for (;i< seccionPool.size();i++)
    {
        if(seccionPool.at(i)->SectionName() == name)
        {
             Section * sec =  seccionPool.takeAt(i);
             sec->deleteLater();
             break;
        }
    }
    if(!seccionPool.isEmpty())
        reCalculateSeccion(seccionPool.at(0));
}

void Paper::updatePaper()
{
    for(int i=0;i<seccionPool.size();i++)
    {
        seccionPool.at(i)->setWidth(this->rect().width());
        seccionPool.at(i)->setMargin(this->margin());
    }
    for(int a = 0;a<itemPool.size();a++)
        itemPool.at(a)->setMargins(this->margin());
    this->scene()->setSceneRect(-15,-15,this->rect().width()+100,this->rect().height()+30);
}

void Paper::newDoc()
{
    this->setOrientacion(Paper::Retrato);
    this->updatePaper();
    QListIterator<Section*> secIt(seccionPool);
    QListIterator<Container*> itemIt(itemPool);
    while(secIt.hasNext())
        secIt.next()->deleteLater();
    while(itemIt.hasNext())
        itemIt.next()->deleteLater();
    seccionPool.clear();
    itemPool.clear();
    this->scene()->update();
}
