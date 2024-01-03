package edu.stevens.cs548.clinic.service.web.rest;

import java.util.Date;
import java.util.List;

import javax.ws.rs.core.UriBuilder;
import javax.ws.rs.core.UriInfo;

import edu.stevens.cs548.clinic.domain.ITreatmentVisitor;
import edu.stevens.cs548.clinic.domain.RadDate;
import edu.stevens.cs548.clinic.service.dto.DrugTreatmentType;
import edu.stevens.cs548.clinic.service.web.rest.data.LinkType;

public class Treatment_PDO_to_Representation implements ITreatmentVisitor {

	private TreatmentRepresentation rep;
	private UriInfo uriInfo;
	
	public Treatment_PDO_to_Representation(UriInfo uriInfo) {
		this.uriInfo = uriInfo;
	}
	public TreatmentRepresentation getRep() {
		return rep;
	}
	
	@Override
	public void visitDrugTreatment(long tid, long patientDbId,
			long providerNPI, String diagnosis, String drug, float dosage) {
		
		rep = new TreatmentRepresentation();
		rep.setId(tid);
		
		UriBuilder ub = uriInfo.getBaseUriBuilder();
		ub.path("patient").path("{id}");
		String patientURI = ub.build(Long.toString(patientDbId)).toString();
		LinkType patientLink = new LinkType();
		patientLink.setUrl(patientURI);
		patientLink.setRelation(Representation.RELATION_PATIENT);
		patientLink.setMediaType(Representation.MEDIA_TYPE);
		rep.setPatient(patientLink);
		
		UriBuilder ub2 = uriInfo.getBaseUriBuilder();
		ub2.path("provider").path("byNPI").queryParam("id", Long.toString(providerNPI));
		String providerURI = ub2.build().toString();
		LinkType providerLink = new LinkType();
		providerLink.setUrl(providerURI);
		providerLink.setRelation(Representation.RELATION_PROVIDER);
		providerLink.setMediaType(Representation.MEDIA_TYPE);
		rep.setProvider(providerLink);
		
		rep.setDiagnosis(diagnosis);
		
		DrugTreatmentRepresentation drugRep = new DrugTreatmentRepresentation();
		drugRep.setDrugName(drug);
		drugRep.setDosage(dosage);
		rep.setDrugTreatment(drugRep);
		
	}

	@Override
	public void visitRadiologyTreatment(long tid, long patientDbId,
			long providerNPI, String diagnosis, List<RadDate> dates) {
		
		rep = new TreatmentRepresentation();
		rep.setId(tid);
		
		UriBuilder ub = uriInfo.getBaseUriBuilder();
		ub.path("patient").path("{id}");
		String patientURI = ub.build(Long.toString(patientDbId)).toString();
		LinkType patientLink = new LinkType();
		patientLink.setUrl(patientURI);
		patientLink.setRelation(Representation.RELATION_PATIENT);
		patientLink.setMediaType(Representation.MEDIA_TYPE);
		rep.setPatient(patientLink);
		
		UriBuilder ub2 = uriInfo.getBaseUriBuilder();
		ub2.path("provider").path("byNPI").queryParam("id", Long.toString(providerNPI));
		String providerURI = ub2.build().toString();
		LinkType providerLink = new LinkType();
		providerLink.setUrl(providerURI);
		providerLink.setRelation(Representation.RELATION_PROVIDER);
		providerLink.setMediaType(Representation.MEDIA_TYPE);
		rep.setProvider(providerLink);
		
		rep.setDiagnosis(diagnosis);
		
		RadiologyTreatmentRepresentation radiologyRep = new RadiologyTreatmentRepresentation();
		List<Date> dateList = radiologyRep.getDate();
		for(RadDate radDate : dates) {
			dateList.add(radDate.getDate());
		}
		rep.setRadiologyTreatment(radiologyRep);
		
	}

	@Override
	public void visitSurgeryTreatment(long tid, long patientDbId,
			long providerNPI, String diagnosis, Date date) {
		
		rep = new TreatmentRepresentation();
		rep.setId(tid);
		
		UriBuilder ub = uriInfo.getBaseUriBuilder();
		ub.path("patient").path("{id}");
		String patientURI = ub.build(Long.toString(patientDbId)).toString();
		LinkType patientLink = new LinkType();
		patientLink.setUrl(patientURI);
		patientLink.setRelation(Representation.RELATION_PATIENT);
		patientLink.setMediaType(Representation.MEDIA_TYPE);
		rep.setPatient(patientLink);
		
		UriBuilder ub2 = uriInfo.getBaseUriBuilder();
		ub2.path("provider").path("byNPI").queryParam("id", Long.toString(providerNPI));
		String providerURI = ub2.build().toString();
		LinkType providerLink = new LinkType();
		providerLink.setUrl(providerURI);
		providerLink.setRelation(Representation.RELATION_PROVIDER);
		providerLink.setMediaType(Representation.MEDIA_TYPE);
		rep.setProvider(providerLink);
		
		rep.setDiagnosis(diagnosis);
		
		SurgeryTreatmentRepresentation surgeryRep = new SurgeryTreatmentRepresentation();
		surgeryRep.setDate(date);
		rep.setSurgeryTreatment(surgeryRep);
		
	}

}
