//
// ���ļ����� JavaTM Architecture for XML Binding (JAXB) ����ʵ�� v2.2.7 ���ɵ�
// ����� <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// �����±���Դģʽʱ, �Դ��ļ��������޸Ķ�����ʧ��
// ����ʱ��: 2014.10.11 ʱ�� 11:52:53 PM EDT 
//


package edu.stevens.cs548.clinic.service.dto;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>anonymous complex type�� Java �ࡣ
 * 
 * <p>����ģʽƬ��ָ�������ڴ����е�Ԥ�����ݡ�
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
     * ��ȡpatientDbId���Ե�ֵ��
     * 
     */
    public long getPatientDbId() {
        return patientDbId;
    }

    /**
     * ����patientDbId���Ե�ֵ��
     * 
     */
    public void setPatientDbId(long value) {
        this.patientDbId = value;
    }

    /**
     * ��ȡproviderNPI���Ե�ֵ��
     * 
     */
    public long getProviderNPI() {
        return providerNPI;
    }

    /**
     * ����providerNPI���Ե�ֵ��
     * 
     */
    public void setProviderNPI(long value) {
        this.providerNPI = value;
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
