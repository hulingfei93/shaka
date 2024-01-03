//
// 此文件是由 JavaTM Architecture for XML Binding (JAXB) 引用实现 v2.2.7 生成的
// 请访问 <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// 在重新编译源模式时, 对此文件的所有修改都将丢失。
// 生成时间: 2014.11.23 时间 07:49:23 PM EST 
//


package edu.stevens.cs548.clinic.service.web.rest.data;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>TreatmentType complex type的 Java 类。
 * 
 * <p>以下模式片段指定包含在此类中的预期内容。
 * 
 * <pre>
 * &lt;complexType name="TreatmentType">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}long"/>
 *         &lt;element name="patient" type="{http://cs548.stevens.edu/clinic/service/web/rest/data/dap}LinkType"/>
 *         &lt;element name="provider" type="{http://cs548.stevens.edu/clinic/service/web/rest/data/dap}LinkType"/>
 *         &lt;element name="diagnosis" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;choice>
 *           &lt;element name="drugTreatment" type="{http://cs548.stevens.edu/clinic/service/web/rest/data}DrugTreatmentType"/>
 *           &lt;element name="surgeryTreatment" type="{http://cs548.stevens.edu/clinic/service/web/rest/data}SurgeryTreatmentType"/>
 *           &lt;element name="radiologyTreatment" type="{http://cs548.stevens.edu/clinic/service/web/rest/data}RadiologyTreatmentType"/>
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
@XmlType(name = "TreatmentType", propOrder = {
    "id",
    "patient",
    "provider",
    "diagnosis",
    "drugTreatment",
    "surgeryTreatment",
    "radiologyTreatment"
})
public class TreatmentType {

    protected long id;
    @XmlElement(required = true)
    protected LinkType patient;
    @XmlElement(required = true)
    protected LinkType provider;
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
     * 获取patient属性的值。
     * 
     * @return
     *     possible object is
     *     {@link LinkType }
     *     
     */
    public LinkType getPatient() {
        return patient;
    }

    /**
     * 设置patient属性的值。
     * 
     * @param value
     *     allowed object is
     *     {@link LinkType }
     *     
     */
    public void setPatient(LinkType value) {
        this.patient = value;
    }

    /**
     * 获取provider属性的值。
     * 
     * @return
     *     possible object is
     *     {@link LinkType }
     *     
     */
    public LinkType getProvider() {
        return provider;
    }

    /**
     * 设置provider属性的值。
     * 
     * @param value
     *     allowed object is
     *     {@link LinkType }
     *     
     */
    public void setProvider(LinkType value) {
        this.provider = value;
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
