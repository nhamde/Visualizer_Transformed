#include <QFileDialog>
#include <QGridLayout>
#include "Visualizer.h"
#include "STLReader.h"
#include "OBJReader.h"
#include "OBJWriter.h"
#include "STLWriter.h"
#include "DataWriter.h"
#include <QDebug>
#include "Transformation.h"
using namespace Geometry;
using namespace transformya;

Visualizer::Visualizer(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();

    connect(loadFile, &QPushButton::clicked, this, &Visualizer::onLoadFileClick);
    connect(translate, &QPushButton::clicked, this, &Visualizer::onTranslateClick);
    connect(exportFile, &QPushButton::clicked, this, &Visualizer::onExportClick);
}

Visualizer::~Visualizer()
{

}

void Visualizer::setupUi()
{
    loadFile = new QPushButton("Load File", this);
    translate = new QPushButton("Translate", this);
    exportFile= new QPushButton("Export", this);
    openglWidgetInput = new OpenGlWidget(this);
    openglWidgetOutput = new OpenGlWidget(this);

    graphicsSynchronizer = new GraphicsSynchronizer(openglWidgetInput, openglWidgetOutput);

    QGridLayout* layout = new QGridLayout(this);

    layout->addWidget(loadFile, 0, 0, 1, 2);
    layout->addWidget(translate, 0, 2, 1, 2);
    layout->addWidget(exportFile, 0, 4, 1, 2);
    layout->addWidget(openglWidgetInput, 1, 0, 1, 3);
    layout->addWidget(openglWidgetOutput, 1, 3, 1, 3);
    
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);
}

void Visualizer::readFile(const QString& inFileName)
{
    if (inFileName.endsWith(".stl", Qt::CaseInsensitive))
    {
        STLReader reader;
        reader.read(inFileName.toStdString(), triangulation);
    }
    else if (inFileName.endsWith(".obj", Qt::CaseInsensitive))
    {
        OBJReader reader;
        reader.read(inFileName.toStdString(), triangulation);
    }
}

void Visualizer::writeFile()
{
    if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"), "", tr("files (*.obj)"));
        qInfo() << fileName << endl;
        ObjWriter writer;
        writer.Write(fileName.toStdString(), triangulation);
    }
    else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"), "", tr("files (*.stl)"));
        STLWriter writer;
        writer.Write(fileName.toStdString(), triangulation);
    }
}

void  Visualizer::onLoadFileClick()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "", tr("files (*.stl *.obj)"));

    if (!fileName.isEmpty())
    {
        inputFilePath = fileName;
        readFile(inputFilePath);
        OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(triangulation);
        openglWidgetInput->setData(data);
        if (secondTime == true)
        {
            data.vertices.clear();
            data.normals.clear();
            openglWidgetInput->update();
        }
        secondTime = true;
    }
    else
    {
        msgBox.setText("No file was selected");
        msgBox.exec();
    }
}

void Visualizer::onTranslateClick()
{
    if (!inputFilePath.isEmpty())
    {
        QFileDialog dialog(this);

        dialog.setFileMode(QFileDialog::Directory);

        QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
            "/home",
            QFileDialog::ShowDirsOnly
            | QFileDialog::DontResolveSymlinks);

        // Check if directory is selected
        if (!dir.isEmpty())
        {
            QString exportFileName;

            if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
            {
                exportFileName = QDir(dir).filePath("temp.obj");
                ObjWriter writer;
                writer.Write(exportFileName.toStdString(), triangulation);

                transformya::Transformation transformer;
                Triangulation tr = transformer.scale(triangulation, 5.0);
                /*qInfo() << tr.UniqueNumbers.size();
                qInfo() << triangulation.UniqueNumbers.size();*/

                /*for (int i = 0; i < triangulation.UniqueNumbers.size(); i++)
                    qInfo() << triangulation.UniqueNumbers[i] << endl;
                qInfo() << "Now next" << endl;
                for (int i = 0; i < tr.UniqueNumbers.size(); i++)
                    qInfo() << tr.UniqueNumbers[i] << endl;*/
                // reload file to check and load in output renderer
                OBJReader reader;
                reader.read(exportFileName.toStdString(), tr);

                QFile::remove(exportFileName);
            }

            else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
            {
                exportFileName = QDir(dir).filePath("temp.stl");
                STLWriter writer;
                writer.Write(exportFileName.toStdString(), triangulation);

                // reload file to check and load in output renderer
                STLReader reader;
                reader.read(exportFileName.toStdString(), triangulation);

                QFile::remove(exportFileName);
            }

            OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(triangulation);
            openglWidgetOutput->setData(data);
        }
        else
        {
            msgBox.setText("No directory was selected!");
            msgBox.exec();
        }
    }
    else
    {
        msgBox.setText("Input file was not loaded!");
        msgBox.exec();
    }
}

void Visualizer::onExportClick()
{
    if (!inputFilePath.isEmpty())
    {
        QFileDialog dialog(this);
        writeFile();
    }
    else
    {
        msgBox.setText("Input file was not loaded!");
        msgBox.exec();
    }
}

OpenGlWidget::Data Visualizer::convertTrianglulationToGraphicsObject(const Triangulation& triangulation)
{
    OpenGlWidget::Data data;
    for  (Triangle triangle : triangulation.Triangles)
    {
        for each (Point point in triangle.Points())
        {
            data.vertices.push_back(triangulation.UniqueNumbers[point.X()]);
            data.vertices.push_back(triangulation.UniqueNumbers[point.Y()]);
            data.vertices.push_back(triangulation.UniqueNumbers[point.Z()]);
        }

        Point normal = triangle.Normal();

        for (size_t i = 0; i < 3; i++)
        {
            data.normals.push_back(triangulation.UniqueNumbers[normal.X()]);
            data.normals.push_back(triangulation.UniqueNumbers[normal.Y()]);
            data.normals.push_back(triangulation.UniqueNumbers[normal.Z()]);
        }
    }
    return data;
}
