//
// 此文件是由 JavaTM Architecture for XML Binding (JAXB) 引用实现 v2.2.7 生成的
// 请访问 <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// 在重新编译源模式时, 对此文件的所有修改都将丢失。
// 生成时间: 2014.10.11 时间 11:52:53 PM EDT 
//


package edu.stevens.cs548.clinic.service.dto;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>anonymous complex type的 Java 类。
 * 
 * <p>以下模式片段指定包含在此类中的预期内容。
 * 
 * <pre>
 * &lt;complexType>
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}long"/>
 *         &lt;element name="patientDbId" type="{http://www.w3.org/2001/XMLSchema}long"/>
 *         &lt;element name="providerNPI" type="{http://www.w3.org/2001/XMLSchema}long"/>
 *         &lt;element name="diagnosis" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;choice>
 *           &lt;element name="drugTreatment" type="{http://www.example.org/clinic/schemas/treatment-dto}DrugTreatmentType"/>
 *           &lt;element name="surgeryTreatment" type="{http://www.example.org/clinic/schemas/treatment-dto}SurgeryTreatmentType"/>
 *           &lt;element name="radiologyTreatment" type="{http://www.example.org/clinic/schemas/treatment-dto}RadiologyTreatmentType"/>
 *         &lt;/choice>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "", propOrder = {
    "id",
    "patientDbId",
    "providerNPI",
    "diagnosis",
    "drugTreatment",
    "surgeryTreatment",
    "radiologyTreatment"
})
@XmlRootElement(name = "TreatmentDTO")
public class TreatmentDTO {

    protected long id;
    protected long patientDbId;
    protected long providerNPI;
    @XmlElement(required = true)
    protected String diagnosis;
    protected DrugTreatmentType drugTreatment;
    protected SurgeryTreatmentType surgeryTreatment;
    protected RadiologyTreatmentType radiologyTreatment;

    /**
     * 获取id属性的值。
     * 
     */
    public long getId() {
        return id;
    }

    /**
     * 设置id属性的值。
     * 
     */
    public void setId(long value) {
        this.id = value;
    }

    /**
     * 获取patientDbId属性的值。
     * 
     */
    public long getPatientDbId() {
        return patientDbId;
    }

    /**
     * 设置patientDbId属性的值。
     * 
     */
    public void setPatientDbId(long value) {
        this.patientDbId = value;
    }

    /**
     * 获取providerNPI属性的值。
     * 
     */
    public long getProviderNPI() {
        return providerNPI;
    }

    /**
     * 设置providerNPI属性的值。
     * 
     */
    public void setProviderNPI(long value) {
        this.providerNPI = value;
    }

    /**
     * 获取diagnosis属性的值。
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getDiagnosis() {
        return diagnosis;
    }

    /**
     * 设置diagnosis属性的值。
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setDiagnosis(String value) {
        this.diagnosis = value;
    }

    /**
     * 获取drugTreatment属性的值。
     * 
     * @return
     *     possible object is
     *     {@link DrugTreatmentType }
     *     
     */
    public DrugTreatmentType getDrugTreatment() {
        return drugTreatment;
    }

    /**
     * 设置drugTreatment属性的值。
     * 
     * @param value
     *     allowed object is
     *     {@link DrugTreatmentType }
     *     
     */
    public void setDrugTreatment(DrugTreatmentType value) {
        this.drugTreatment = value;
    }

    /**
     * 获取surgeryTreatment属性的值。
     * 
     * @return
     *     possible object is
     *     {@link SurgeryTreatmentType }
     *     
     */
    public SurgeryTreatmentType getSurgeryTreatment() {
        return surgeryTreatment;
    }

    /**
     * 设置surgeryTreatment属性的值。
     * 
     * @param value
     *     allowed object is
     *     {@link SurgeryTreatmentType }
     *     
     */
    public void setSurgeryTreatment(SurgeryTreatmentType value) {
        this.surgeryTreatment = value;
    }

    /**
     * 获取radiologyTreatment属性的值。
     * 
     * @return
     *     possible object is
     *     {@link RadiologyTreatmentType }
     *     
     */
    public RadiologyTreatmentType getRadiologyTreatment() {
        return radiologyTreatment;
    }

    /**
     * 设置radiologyTreatment属性的值。
     * 
     * @param value
     *     allowed object is
     *     {@link RadiologyTreatmentType }
     *     
     */
    public void setRadiologyTreatment(RadiologyTreatmentType value) {
        this.radiologyTreatment = value;
    }

}
