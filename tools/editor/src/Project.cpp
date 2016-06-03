#include "Project.h"
#include <QCoreApplication>

#define PROJECT_FILE "game.project"
#define CONFIG_FILE "game.config"
#define RESOURCE_DIR "res"
#define SCENE_EXT ".scene"
#define SCENE_NEW "Scene"
#define NODE_NEW "Node"

bool Project::_serializerActivated = false;


Project::Project(QObject* parent) : QFileSystemModel(parent)
{
}

Project::~Project()
{
}

Project* Project::create(const QString& path, const QString& name, QObject* parent)
{
    Project* project = new Project(parent);
    project->_name = name.toStdString();
    project->_path = path.toStdString();
    project->_scenePath = std::string(RESOURCE_DIR) + std::string("/") + std::string(SCENE_NEW) + std::string(SCENE_EXT);

    // Create a resource directory
    QDir projectDir(path);
    if (!projectDir.mkdir("res"))
    {
        SAFE_DELETE(project);
        return nullptr;
    }
    // Create a project file
    if (!_serializerActivated)
    {
        Serializer::getActivator()->registerClass("gameplay::Project", &Project::createInstance);
        _serializerActivated = true;
    }
    QString projectFilePath(path + QString("/") + QString(QLatin1String(PROJECT_FILE)));
    QByteArray projectFilePathByteArray = projectFilePath.toLatin1();
    const char* projectFilePathStr = projectFilePathByteArray.data();
    Serializer* projectWriter = SerializerJson::createWriter(projectFilePathStr);
    projectWriter->writeObject(nullptr, project);
    projectWriter->close();
    SAFE_DELETE(projectWriter);

    // Create a config file
    QString configFilePath(path + QString("/") + QString(QLatin1String(CONFIG_FILE)));
    QByteArray configFilePathByteArray = configFilePath.toLatin1();
    const char* configFilePathStr = configFilePathByteArray.data();
    Serializer* configWriter = SerializerJson::createWriter(configFilePathStr);
    Game::Config config;
    configWriter->writeObject(nullptr, &config);
    configWriter->close();
    SAFE_DELETE(configWriter);

    // Create an empty scene.
    std::string id = SCENE_NEW;
    Scene* scene = Scene::create(id.c_str());

    // Add a perspective camera
    float aspectRatio = (float)config.width / (float)config.height;
    Camera* camera = Camera::createPerspective(45.0f, aspectRatio, 1.0f, 10.0f);
    Node* cameraNode = scene->addNode(NODE_NEW);
    cameraNode->setCamera(camera);
    scene->setActiveCamera(camera);
    SAFE_RELEASE(camera);

    // Add a directional light
    Light* light = Light::createDirectional(Vector3::one());
    Node* lightNode = Node::create(NODE_NEW);
    lightNode->setLight(light);
    lightNode->setTranslation(0.0f, 0.0f, 10.0f);
    SAFE_RELEASE(light);
    scene->addNode(lightNode);

    QString sceneFilePath(path + QDir::separator() + QString(QLatin1String(RESOURCE_DIR)) +
                                 QDir::separator() + QString(QLatin1String(SCENE_NEW)) + QString(QLatin1String(SCENE_EXT)));
    QByteArray sceneFilePathByteArray = sceneFilePath.toLatin1();
    const char* sceneFilePathStr = sceneFilePathByteArray.data();
    Serializer* sceneWriter = SerializerJson::createWriter(sceneFilePathStr);
    sceneWriter->writeObject(nullptr, scene);
    sceneWriter->close();
    SAFE_DELETE(sceneWriter)
    SAFE_RELEASE(scene);

    return project;
}

Project* Project::open(const QString& path, QObject* parent)
{
    if (!_serializerActivated)
    {
        Serializer::getActivator()->registerClass("gameplay::Project", &Project::createInstance);
        _serializerActivated = true;
    }

    QString projectPath(path + QString("/") + QString(QLatin1String(PROJECT_FILE)));
    if (!QFile(projectPath).exists())
        return nullptr;

    QByteArray ba = projectPath.toLatin1();
    const char* str = ba.data();
    Serializer* projectReader = Serializer::createReader(str);
    Project* project = nullptr;
    if (projectReader)
    {
        project = static_cast<Project*>(projectReader->readObject(nullptr));
        projectReader->close();
        SAFE_DELETE(projectReader);
    }
    return project;
}

QString Project::getPath()
{
    return QString::fromStdString(_path);
}

QString Project::getName()
{
    return QString::fromStdString(_name);
}

QString Project::getScenePath()
{
    return QString::fromStdString(_scenePath);
}

void Project::setScenePath(const QString& scenePath)
{
    _scenePath = scenePath.toStdString();
}

QVariant Project::data(const QModelIndex& index, int role) const
{
    switch(role)
    {
        case Qt::TextAlignmentRole:
            return Qt::AlignBottom;
        case Qt::DecorationRole:
            {
                if (index.column() == 0)
                {
                    QFileInfo info = Project::fileInfo(index);
                    if(info.isFile())
                    {
                        if(info.suffix() == "config")
                            return QPixmap(":/res/images/project-config.png");
                        else if(info.suffix() == "scene")
                            return QPixmap(":/res/images/project-scene.png");
                    }
                }
            }
            break;
    }
    return QFileSystemModel::data(index, role);
}

Serializable* Project::createInstance()
{
    return static_cast<Project*>(new Project());
}

const char* Project::getSerializedClassName() const
{
    return "gameplay::Project";
}

void Project::serialize(Serializer* serializer)
{
    serializer->writeString("name", _name.c_str(), "");
    serializer->writeString("scene", _scenePath.c_str(), "");
}

void Project::deserialize(Serializer* serializer)
{
    serializer->readString("name", _name, "" );
    serializer->readString("scene", _scenePath, "");

    _path = serializer->getPath();
}

