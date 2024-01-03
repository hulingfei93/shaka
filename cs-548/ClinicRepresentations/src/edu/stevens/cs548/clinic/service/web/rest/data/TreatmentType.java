//
// ���ļ����� JavaTM Architecture for XML Binding (JAXB) ����ʵ�� v2.2.7 ���ɵ�
// ����� <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// �����±���Դģʽʱ, �Դ��ļ��������޸Ķ�����ʧ��
// ����ʱ��: 2014.11.23 ʱ�� 07:49:23 PM EST 
//


package edu.stevens.cs548.clinic.service.web.rest.data;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>TreatmentType complex type�� Java �ࡣ
 * 
 * <p>����ģʽƬ��ָ�������ڴ����е�Ԥ�����ݡ�
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
     * ��ȡid���Ե�ֵ��
     * 
     */
    public long getId() {
        return id;
    }

    /**
     * ����id���Ե�ֵ��
     * 
     */
    public void setId(long value) {
        this.id = value;
    }

    /**
     * ��ȡpatient���Ե�ֵ��
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
     * ����patient���Ե�ֵ��
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
     * ��ȡprovider���Ե�ֵ��
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
     * ����provider���Ե�ֵ��
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
     * ��ȡdiagnosis���Ե�ֵ��
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
     * ����diagnosis���Ե�ֵ��
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
     * ��ȡdrugTreatment���Ե�ֵ��
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
     * ����drugTreatment���Ե�ֵ��
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
     * ��ȡsurgeryTreatment���Ե�ֵ��
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
     * ����surgeryTreatment���Ե�ֵ��
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
     * ��ȡradiologyTreatment���Ե�ֵ��
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
     * ����radiologyTreatment���Ե�ֵ��
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
