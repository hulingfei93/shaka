//
// 此文件是由 JavaTM Architecture for XML Binding (JAXB) 引用实现 v2.2.7 生成的
// 请访问 <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// 在重新编译源模式时, 对此文件的所有修改都将丢失。
// 生成时间: 2014.11.23 时间 07:49:23 PM EST 
//


package edu.stevens.cs548.clinic.service.web.rest.data;

import javax.xml.bind.JAXBElement;
import javax.xml.bind.annotation.XmlElementDecl;
import javax.xml.bind.annotation.XmlRegistry;
import javax.xml.namespace.QName;
import edu.stevens.cs548.clinic.service.web.rest.PatientRepresentation;
import edu.stevens.cs548.clinic.service.web.rest.ProviderRepresentation;
import edu.stevens.cs548.clinic.service.web.rest.TreatmentRepresentation;


/**
 * This object contains factory methods for each 
 * Java content interface and Java element interface 
 * generated in the edu.stevens.cs548.clinic.service.web.rest.data package. 
 * <p>An ObjectFactory allows you to programatically 
 * construct new instances of the Java representation 
 * for XML content. The Java representation of XML 
 * content can consist of schema derived interfaces 
 * and classes representing the binding of schema 
 * type definitions, element declarations and model 
 * groups.  Factory methods for each of these are 
 * provided in this class.
 * 
 */
@XmlRegistry
public class ObjectFactory {

    private final static QName _ProviderRep_QNAME = new QName("http://cs548.stevens.edu/clinic/service/web/rest/data", "provider-rep");
    private final static QName _TreatmentRep_QNAME = new QName("http://cs548.stevens.edu/clinic/service/web/rest/data", "treatment-rep");
    private final static QName _PatientRep_QNAME = new QName("http://cs548.stevens.edu/clinic/service/web/rest/data", "patient-rep");

    /**
     * Create a new ObjectFactory that can be used to create new instances of schema derived classes for package: edu.stevens.cs548.clinic.service.web.rest.data
     * 
     */
    public ObjectFactory() {
    }

    /**
     * Create an instance of {@link PatientType }
     * 
     */
    public PatientType createPatientType() {
        return new PatientRepresentation();
    }

    /**
     * Create an instance of {@link TreatmentType }
     * 
     */
    public TreatmentType createTreatmentType() {
        return new TreatmentRepresentation();
    }

    /**
     * Create an instance of {@link ProviderType }
     * 
     */
    public ProviderType createProviderType() {
        return new ProviderRepresentation();
    }

    /**
     * Create an instance of {@link RadiologyTreatmentType }
     * 
     */
    public RadiologyTreatmentType createRadiologyTreatmentType() {
        return new RadiologyTreatmentType();
    }

    /**
     * Create an instance of {@link DrugTreatmentType }
     * 
     */
    public DrugTreatmentType createDrugTreatmentType() {
        return new DrugTreatmentType();
    }

    /**
     * Create an instance of {@link SurgeryTreatmentType }
     * 
     */
    public SurgeryTreatmentType createSurgeryTreatmentType() {
        return new SurgeryTreatmentType();
    }

    /**
     * Create an instance of {@link LinkType }
     * 
     */
    public LinkType createLinkType() {
        return new LinkType();
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ProviderType }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://cs548.stevens.edu/clinic/service/web/rest/data", name = "provider-rep")
    public JAXBElement<ProviderType> createProviderRep(ProviderType value) {
        return new JAXBElement<ProviderType>(_ProviderRep_QNAME, ((Class) ProviderRepresentation.class), null, ((ProviderRepresentation) value));
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link TreatmentType }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://cs548.stevens.edu/clinic/service/web/rest/data", name = "treatment-rep")
    public JAXBElement<TreatmentType> createTreatmentRep(TreatmentType value) {
        return new JAXBElement<TreatmentType>(_TreatmentRep_QNAME, ((Class) TreatmentRepresentation.class), null, ((TreatmentRepresentation) value));
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link PatientType }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://cs548.stevens.edu/clinic/service/web/rest/data", name = "patient-rep")
    public JAXBElement<PatientType> createPatientRep(PatientType value) {
        return new JAXBElement<PatientType>(_PatientRep_QNAME, ((Class) PatientRepresentation.class), null, ((PatientRepresentation) value));
    }

}
