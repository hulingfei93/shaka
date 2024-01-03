package edu.stevens.cs548.clinic.domain;

import java.io.Serializable;
import java.util.Date;

import javax.persistence.*;

/**
 * Entity implementation class for Entity: SurgeryTreatment
 *
 */

@Entity
@Table(name = "surgeryTreatment")
@DiscriminatorValue("S")
public class SurgeryTreatment extends Treatment implements Serializable {

	private static final long serialVersionUID = 1L;

	@Temporal(TemporalType.DATE)
	private Date date;

	public Date getDate() {
		return date;
	}

	public void setDate(Date date) {
		this.date = date;
	}

	@Override
	public void visit(ITreatmentVisitor visitor) {
		visitor.visitSurgeryTreatment(this.getId(), this.getPatient().getId(), this.getProvider().getNPI(), this.getDiagnosis(), this.date);
	}

	public SurgeryTreatment() {
		super();
		setTreatmentType("D");
	}
}
