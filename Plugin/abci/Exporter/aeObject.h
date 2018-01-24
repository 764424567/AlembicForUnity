#pragma once

class aeProperty
{
public:
    aeProperty();
    virtual ~aeProperty();
    virtual const char* getName() const = 0;
    virtual bool isArray() const = 0; // true if property is array (abcFloatArrayProperty etc.)
    virtual void writeSample(const void *data, int data_num) = 0; // data_num is ignored on scalar property
};


class aeObject
{
public:
    aeObject(aeContext *ctx, aeObject *parent, abcObject *abc, uint32_t tsi);
    virtual ~aeObject();

    const char* getName() const;
    const char* getFullName() const;
    uint32_t    getTimeSamplingIndex() const;
    size_t      getNumChildren() const;
    aeObject*   getChild(int i);
    aeObject*   getParent();

    aeContext*          getContext();
    const aeConfig&     getConfig() const;
    virtual abcObject&  getAbcObject();
    virtual abcProperties getAbcProperties();

    /// T: aeCamera, aeXForm, aePoint, aePolyMesh
    template<class T> T*    newChild(const char *name, uint32_t tsi = 1);
    void                    removeChild(aeObject *c);

    /// T: abcFloatArrayProperty, abcFloatProperty, etc
    template<class T>
    aeProperty*             newProperty(const char *name, uint32_t tsi = 1);

    virtual size_t  getNumSamples();
    virtual void    setFromPrevious();

protected:
    using aePropertyPtr = std::unique_ptr<aeProperty>;
    aeContext                   *m_ctx = nullptr;
    aeObject                    *m_parent = nullptr;
    uint32_t                    m_tsi = 0;
    std::unique_ptr<abcObject>  m_abc;
    std::vector<aePropertyPtr>  m_properties;
    std::vector<aeObject*>      m_children;
};
