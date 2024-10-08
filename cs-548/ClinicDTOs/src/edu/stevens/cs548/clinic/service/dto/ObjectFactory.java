//
// 此文件是由 JavaTM Architecture for XML Binding (JAXB) 引用实现 v2.2.7 生成的
// 请访问 <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// 在重新编译源模式时, 对此文件的所有修改都将丢失。
// 生成时间: 2014.10.11 时间 11:52:53 PM EDT 
//


package edu.stevens.cs548.clinic.service.dto;

import javax.xml.bind.annotation.XmlRegistry;


/**
 * This object contains factory methods for each 
 * Java content interface and Java element interface 
 * generated in the edu.stevens.cs548.clinic.service.dto package. 
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


    /**
     * Create a new ObjectFactory that can be used to create new instances of schema derived classes for package: edu.stevens.cs548.clinic.service.dto
     * 
     */
    public ObjectFactory() {
    }

    public PatientDTO createPatientDTO() {
        return new PatientDTO();
    }
    
    public ProviderDTO createProviderDTO() {
        return new ProviderDTO();
    }
    
    /**
     * Create an instance of {@link TreatmentDTO }
     * 
     */
    public TreatmentDTO createTreatmentDTO() {
        return new TreatmentDTO();
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
     * Create an instance of {@link RadiologyTreatmentType }
     * 
     */
    public RadiologyTreatmentType createRadiologyTreatmentType() {
        return new RadiologyTreatmentType();
    }
}
