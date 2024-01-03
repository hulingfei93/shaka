//package edu.stevens.cs548.clinic.test;
//
//import java.text.SimpleDateFormat;
//import java.util.Date;
//import java.util.List;
//import java.util.logging.Logger;
//
//import javax.annotation.PostConstruct;
//import javax.ejb.EJB;
//import javax.ejb.LocalBean;
//import javax.ejb.Singleton;
//import javax.ejb.Startup;
//import javax.inject.Inject;
//
//import edu.stevens.cs548.clinic.domain.Provider;
//import edu.stevens.cs548.clinic.domain.ProviderSpecEnum;
//import edu.stevens.cs548.clinic.service.dto.DrugTreatmentType;
//import edu.stevens.cs548.clinic.service.dto.ObjectFactory;
//import edu.stevens.cs548.clinic.service.dto.PatientDTO;
//import edu.stevens.cs548.clinic.service.dto.ProviderDTO;
//import edu.stevens.cs548.clinic.service.dto.RadiologyTreatmentType;
//import edu.stevens.cs548.clinic.service.dto.SurgeryTreatmentType;
//import edu.stevens.cs548.clinic.service.dto.TreatmentDTO;
//import edu.stevens.cs548.clinic.service.ejb.IPatientServiceLocal;
//import edu.stevens.cs548.clinic.service.ejb.IProviderServiceLocal;
//
///**
// * Session Bean implementation class TestService
// */
//@Singleton
//@LocalBean
//@Startup
//public class TestService2 {
//
//	private static Logger logger = Logger.getLogger(TestService2.class.getCanonicalName());
//	
//	private static void info(String m) {
//		logger.info(m);
//	}
//
//    public TestService2() { }
//    
//    @EJB(beanName="PatientServiceBean")
//    //@Inject
//    private IPatientServiceLocal patientService;
//    
//    @EJB(beanName="ProviderServiceBean")
//    //@Inject
//    private IProviderServiceLocal providerService;
//    
//    @PostConstruct
//    public void init() {
//    	try {
//    		info("Initializing the service.");
//    		info("-----------Begin-------------");
//    		
//    		SimpleDateFormat sdf = new SimpleDateFormat( "yyyy-MM-dd");
//
//    		// add several patients
//    		long patientDbId1, patientDbId2, patientDbId3;
//    		patientDbId1 = patientService.createPatient("Brown", sdf.parse("1965-05-06"), 49, 357278);
//    		info("Patient: id = " + patientDbId1 + ", pid = " + 357278 + " has been inserted into DB.");
//    		
//    		patientDbId2 = patientService.createPatient("Jones", sdf.parse("1988-03-18"), 26, 848032);
//    		info("Patient: id = " + patientDbId2 + ", pid = " + 848032 + " has been inserted into DB.");
//    		
//    		patientDbId3 = patientService.createPatient("Davis", sdf.parse("1996-01-25"), 18, 367267);
//    		info("Patient: id = " + patientDbId3 + ", pid = " + 367267 + " has been inserted into DB.");
//    		
//    		// add several providers
//    		providerService.createProvider("Smith", 694594);
//    		info("Provider: NPI = " + 694594 + " has been inserted into DB.");
//    		
//    		providerService.createProvider("Johnson", 576834);
//    		info("Provider: NPI = " + 576834 + " has been inserted into DB.");
//    		
//    		providerService.createProvider("Williams", 936767);
//    		info("Provider: NPI = " + 936767 + " has been inserted into DB.");
//    		
//    		// add several treatments
//    		ObjectFactory factory = new ObjectFactory();
//    		
//    		TreatmentDTO treatmentDTO1 = factory.createTreatmentDTO();
//    		treatmentDTO1.setPatientDbId(patientDbId1);
//    		treatmentDTO1.setProviderNPI(576834);
//    		treatmentDTO1.setDiagnosis("code");
//    		DrugTreatmentType drugTreatment = factory.createDrugTreatmentType();
//    		drugTreatment.setDrugName("cold medication");
//    		drugTreatment.setDosage(18.0f);
//    		treatmentDTO1.setDrugTreatment(drugTreatment);
//    		long tid1 = providerService.addTreatment(patientDbId1, 576834, treatmentDTO1);
//    		info("Drug treatment: " + tid1 + " has been inserted into DB.");
//    		
//    		TreatmentDTO treatmentDTO2 = factory.createTreatmentDTO();
//    		treatmentDTO2.setPatientDbId(patientDbId1);
//    		treatmentDTO2.setProviderNPI(576834);
//    		treatmentDTO2.setDiagnosis("code");
//    		RadiologyTreatmentType radiologyTreatment = factory.createRadiologyTreatmentType();
//    		radiologyTreatment.getDate().add(sdf.parse("1986-04-25"));
//    		treatmentDTO2.setRadiologyTreatment(radiologyTreatment);
//    		long tid2 = providerService.addTreatment(patientDbId1, 576834, treatmentDTO2);
//    		info("Radiology treatment: " + tid2 + " has been inserted into DB.");
//    		
//    		TreatmentDTO treatmentDTO3 = factory.createTreatmentDTO();
//    		treatmentDTO3.setPatientDbId(patientDbId1);
//    		treatmentDTO3.setProviderNPI(576834);
//    		treatmentDTO3.setDiagnosis("code");
//    		SurgeryTreatmentType surgeryTreatment = factory.createSurgeryTreatmentType();
//    		surgeryTreatment.setDate(sdf.parse("1996-04-25"));
//    		treatmentDTO3.setSurgeryTreatment(surgeryTreatment);
//    		long tid3 = providerService.addTreatment(patientDbId1, 576834, treatmentDTO3);
//    		info("Surgery treatment: " + tid3 + " has been inserted into DB.");
//    		
//    		PatientDTO patient = patientService.getPatientByDbId(patientDbId1);
//    		ProviderDTO provider = providerService.getProviderByNPI(576834);  		
//    		TreatmentDTO[] treatments = providerService.getTreatments(patient.getId(), provider.getNPI());
//    		info("Treatments of patient: " + patient.getId() + " , supervised by provider: " + provider.getNPI());
//    		for(TreatmentDTO t1 : treatments) {
//    			info(Long.toString(t1.getId()));
//    		}
//    		
//    		// delete treatments
//    		for(TreatmentDTO t2 : treatments) {
//    			providerService.deleteTreatment(t2.getId(), patient.getId(), provider.getNPI());
//    			info("Treatment: " + t2.getId() + " has been deleted from DB.");
//    		}
//    		
//    		
//    		info("-----------End-------------");
//    	}
//    	catch(Exception ex) {
//    		info(ex.getMessage());
//    	}
//    }
//}