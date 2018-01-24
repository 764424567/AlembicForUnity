#pragma once

class aeXForm : public aeObject
{
using super = aeObject;
public:
    aeXForm(aeObject *parent, const char *name, uint32_t tsi);
    abcXForm& getAbcObject() override;
    abcProperties getAbcProperties() override;

    size_t  getNumSamples() override;
    void    setFromPrevious() override;
    void    writeSample(const aeXFormData &data);

private:
    AbcGeom::OXformSchema m_schema;
    AbcGeom::XformSample m_sample;
};
