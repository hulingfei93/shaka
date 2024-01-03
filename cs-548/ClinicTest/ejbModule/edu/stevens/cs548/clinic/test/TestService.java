package edu.stevens.cs548.clinic.test;

import java.text.SimpleDateFormat;
import java.util.logging.Logger;

import javax.annotation.PostConstruct;
import javax.ejb.LocalBean;
import javax.ejb.Singleton;
import javax.ejb.Startup;
import javax.ejb.EJB;

import edu.stevens.cs548.clinic.service.dto.ProviderDTO;
import edu.stevens.cs548.clinic.service.ejb.IProviderServiceLocal;
import edu.stevens.cs548.clinic.service.ejb.ProviderService;
import edu.stevens.cs548.clinic.service.web.rest.DrugTreatmentRepresentation;
import edu.stevens.cs548.clinic.service.web.rest.PatientRepresentation;
import edu.stevens.cs548.clinic.service.web.rest.ProviderRepresentation;
import edu.stevens.cs548.clinic.service.web.rest.TreatmentRepresentation;
import edu.stevens.cs548.clinic.service.web.rest.resources.test.PatientResourceTest;
import edu.stevens.cs548.clinic.service.web.rest.resources.test.ProviderResourceTest;
import edu.stevens.cs548.clinic.service.web.rest.resources.test.TreatmentResourceTest;
import edu.stevens.cs548.clinic.service.web.rest.data.SpeicalizationType;

/**
 * Session Bean implementation class TestService
 */
@Singleton
@LocalBean
@Startup
public class TestService {

	private static Logger logger = Logger.getLogger(TestService.class.getCanonicalName());
	
	private static void info(String m) {
		logger.info(m);
	}

    public TestService() { }
    

    @EJB
    private PatientResourceTest patientRes;  

    @EJB
    private ProviderResourceTest providerRes;

    @EJB
    private TreatmentResourceTest treatmentRes;
    
    @PostConstruct
    public void init() {
    	try {
    		info("-----------Begin-------------");
      	    
    		SimpleDateFormat sdf = new SimpleDateFormat( "yyyy-MM-dd");
    		
    		// create a patient
    		PatientRepresentation patientRep = new PatientRepresentation();
    		patientRep.setName("Brown");
    		patientRep.setDob(sdf.parse("1965-05-06"));
    		patientRep.setPatientId(357278);
    		long patientDbId = patientRes.addPatient(patientRep, 49);
    		info("Patient of id " + patientDbId + " has been inserted into DB.");
    		
    		// create a provider
    		ProviderRepresentation providerRep = new ProviderRepresentation();
    		providerRep.setNPI(694594);
    		providerRep.setName("Smith");
    		long providerNPI = providerRes.addProvider(providerRep);
    		info("provider of NPI " + providerNPI + " has been inserted into DB.");
   		
    		// add a treatment
    		TreatmentRepresentation treatmentRep = new TreatmentRepresentation();
    		treatmentRep.setDiagnosis("code");
    		DrugTreatmentRepresentation drugRep = new DrugTreatmentRepresentation();
    		drugRep.setDrugName("cold medication");
    		drugRep.setDosage(18.0f);
    		treatmentRep.setDrugTreatment(drugRep);
    		long tid = providerRes.addTreatment(Long.toString(patientDbId), Long.toString(providerNPI), treatmentRep);
    		info("The treatment of id " + tid + " has been inserted into DB.");
    		
    		// check
    		PatientRepresentation patient = patientRes.getPatient(Long.toString(patientDbId));
    		ProviderRepresentation provider = providerRes.getProvider(Long.toString(providerNPI));
    		TreatmentRepresentation treatment = treatmentRes.getTreatment(Long.toString(tid));
    		
    		info("Patient of id " + patient.getId() + " contains a treatment of id " + patient.getTreatments().get(0).getUrl());
    		info("Provider of NPI " + provider.getNPI() + " contains a treatment of id " + provider.getTreatments().get(0).getUrl());
    		info("Treatment of id : " + treatment.getId() + " owned by patient of id " + treatment.getLinkPatient().getUrl() 
   				+ " and supervised by provider of id " + treatment.getLinkProvider().getUrl());
    		
    	   
    		
    		info("-----------End-------------");
    	}
    	catch(Exception ex) {
    		info(ex.getMessage());
    	}
    }
}