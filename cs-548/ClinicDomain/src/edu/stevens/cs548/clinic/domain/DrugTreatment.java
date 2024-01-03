package edu.stevens.cs548.clinic.domain;

import java.io.Serializable;

import javax.persistence.*;

/**
 * Entity implementation class for Entity: DrugTreatment
 *
 */
@Entity
@Table(name = "drugTreatment")
@DiscriminatorValue("D")
public class DrugTreatment extends Treatment implements Serializable {

	private static final long serialVersionUID = 1L;

	private String drug;
	private float dosage;

	public String getDrug() {
		return drug;
	}

	public void setDrug(String drug) {
		this.drug = drug;
	}

	public float getDosage() {
		return dosage;
	}

	public void setDosage(float dosage) {
		this.dosage = dosage;
	}

	@Override
	public void visit(ITreatmentVisitor visitor) {
		visitor.visitDrugTreatment(this.getId(), this.getPatient().getId(), this.getProvider().getNPI(), this.getDiagnosis(), this.drug, this.dosage);
	}
	
	public DrugTreatment() {
		super();
		setTreatmentType("D");
	}

}
